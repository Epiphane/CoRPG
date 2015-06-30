#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "server.h"

using namespace std;

Server::Server(const char *filename) : conn(-1) {
	// Copy address info
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, filename);
	strcpy(name,             filename);
}

int Server::init(int max_connections) {
	conn = socket(PF_LOCAL, SOCK_STREAM, 0);
	int res = ::bind(conn, (sockaddr *)&address, sizeof(address));
	if (res < 0) {
		if (errno != EADDRINUSE) {
			cout << "Error binding socket: " << strerror(errno) << endl;
		}

		conn = -1;
		return res;
	}
	
	res = listen(conn, max_connections);
	if (res < 0) {
		cout << "Error listening on socket: " << strerror(errno) << endl;
		conn = -1;
		return res;
	}

	cout << "Server opened on thread " << getpid() << endl;
	
	return res;
}

void Server::release() {
	if (conn >= 0) { 
		cout << "Releasing " << endl;
		shutdown(conn, SHUT_RDWR);
		unlink(name);
	}

	vector<Client>::iterator it;
	for(it = clients.begin();it < clients.end();) {
		it->close_read();
		it = clients.erase(it);
	}
}

void Server::setBlocking(bool blocking) {
	int flags; 

	if ((flags = fcntl(conn, F_GETFL, 0)) < 0) { 
		cerr << "Error getting socket flags: " << strerror(errno) << endl;
	} 

	int res;
	if (!blocking) res = fcntl(conn, F_SETFL, flags | O_NONBLOCK);
	else         res = fcntl(conn, F_SETFL, flags & (~O_NONBLOCK));
	if (res < 0) {
		cerr << "Error setting socket mode: " << strerror(errno) << endl;
	} 
}

Client *Server::accept() {
	int newconn = ::accept(conn, NULL, NULL);

	if (newconn < 0) {
		if (errno != EAGAIN) {
			cerr << "Error accepting connection: " << strerror(errno) << endl;
		}
		return NULL;
	}
	else {
		clients.push_back(Client(newconn));
		return &clients[clients.size() - 1];
	}
}

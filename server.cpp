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

void Server::recv_update(reaction callback) {
	ServerUpdate message;

	vector<Client>::iterator it = clients.begin();
	while (it < clients.end()) {
		ssize_t res = it->recv(&message, sizeof(message), 0);
		if (res == 0) {
			// Connection closed
			it = clients.erase(it);
		}
		else {
			if (res < 0) {
				// Nothing yet...
				if (errno != EAGAIN) {
					cerr << "Error receiving from client: " << strerror(errno) << endl;
				}
			}
			else {
				if (res < sizeof(message)) {
					cerr << "Only " << res << " bytes received / " << sizeof(message) << endl;
				}
				else {
					callback(&message);
				}
			}

			it ++;
		}
	}
}

void Server::send_update(const std::vector<ServerUpdate> &updates) {
	vector<ServerUpdate>::const_iterator upd;
	vector<Client>::iterator       cli;
	// Gross...On^2
	for (upd = updates.begin(); upd < updates.end(); upd ++) {
		for (cli = clients.begin(); cli < clients.end(); cli ++) {
			ssize_t res = cli->send((void *)&(*upd), sizeof(ServerUpdate), 0);
			if (res < 0) {
				cerr << "Error sending to client: " << strerror(errno) << endl;
			}
		}
	}
}

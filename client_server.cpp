#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

#include "client_server.h"

using namespace std;

Client *connect_to_server() {
	const int TIMEOUT = 3;
	
	int conn = socket(PF_LOCAL, SOCK_STREAM, 0);

	struct sockaddr_un address;
	address.sun_family = AF_UNIX;
	strcpy(address.sun_path, SERVER_NAME);

	cout << "Connecting to " << address.sun_path << endl;
	chrono::system_clock::time_point timeout = chrono::system_clock::now() + chrono::duration<int>(TIMEOUT);
	int result;
	do {
		result = connect(conn, (sockaddr *)&address, sizeof(address));
	} while (result < 0 && timeout > chrono::system_clock::now());

	if (result < 0) {
		cout << "Error connecting to server: " << strerror(errno) << endl;
		return NULL;
	}

	// Set socket to nonblocking
	int flags; 

	if ((flags = fcntl(conn, F_GETFL, 0)) < 0) { 
		cerr << "Error getting socket flags: " << strerror(errno) << endl;
	} 

	int res = fcntl(conn, F_SETFL, flags | O_NONBLOCK);
	if (res < 0) {
		cerr << "Error setting socket mode: " << strerror(errno) << endl;
	} 
	
	return new Client(conn);
}

ssize_t Client::send(void *buffer, size_t len, int flags) {
	return ::send(conn, buffer, len, flags);
}

ssize_t Client::recv(void *buffer, size_t len, int flags) {
	return ::recv(conn, buffer, len, flags);
}

void Client::release(int flags) {
	shutdown(conn, flags);
}

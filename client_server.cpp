#include <iostream>
#include <chrono>
#include <thread>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <string.h>
#include <errno.h>

#include "client_server.h"

using namespace std;

int connect_to_server() {
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
	}
	
	return conn;
}

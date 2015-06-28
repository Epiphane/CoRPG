#include <iostream>
#include <errno.h>
#include <fcntl.h>
#include <thread>
#include <chrono>

#include "client_server.h"

using namespace std;

pid_t get_server() {
	// Get timeout for server to kick in
	const int S_TIMEOUT = 2;
	chrono::system_clock::time_point timeout = chrono::system_clock::now() + chrono::duration<int>(S_TIMEOUT);
	int lockfile = -1;
	while (lockfile < 0) {
		lockfile = open(SERVER_LOCK, O_RDONLY);
	
		if (lockfile < 0) {
			if (errno != ENOENT || chrono::system_clock::now() > timeout) {
				cerr << "Error opening server file: " << strerror(errno) << endl;
				return -1;
			}
			else { // File does not exist.
				this_thread::yield();
			}
		}
	}

	pid_t server_pid;
	int numread = read(lockfile, &server_pid, sizeof(pid_t));

	if (numread == -1) {
		cerr << "Error reading server lock: " << strerror(errno) << endl;
		return -1;
	}

	// Set timeout for lock opening
	timeout = chrono::system_clock::now() + chrono::duration<int>(S_TIMEOUT);
	while (numread == 0) {
		numread = read(lockfile, &server_pid, sizeof(pid_t));
		
		if (numread == 0) {
			if (chrono::system_clock::now() > timeout) {
				cerr << "Error reading server lock: Data unavailable" << endl;
				return -1;
			}
			else {
				this_thread::yield();
			}
		}
	}
	
	return server_pid;	
}

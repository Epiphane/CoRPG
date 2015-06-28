#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "client_server.h"
#include "server_lock.h"

using namespace std;

ServerLock s_mutex;

void close_server(int status);
void handle_signal(int sig) {
	if (sig == REQCONNECT) { // Open connection
		cout << "Connection requested" << endl;
	}
	else {
		if (sig == SIGSEGV) {
			cerr << "Segmentation Fault" << endl;
		}

		cout << "Received signal " << sig << ".";

		close_server(1);
	}
}

int serve() {
	fstream server_state("._serverinfo_", iostream::in);
	if (!server_state.is_open()) {
		if (errno == ENOENT) {
			// Clean slate!
			while(1);
		}
		else {
			cerr << "Error opening ._serverinfo_: " << strerror(errno) << endl;
			return 1;
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	for (int arg = 1; arg < argc; arg ++) {
		if (strcmp(argv[arg], "--force") == 0) {	
			s_mutex.unlock(true);
			break;
		}
	}
	
	s_mutex.lock();
	// Get a lock on the server
	if (!s_mutex.is_open()) {
		if (strcmp(argv[0], "funserver") == 0) {
			cout << "Server Locked. Aborting" << endl;
		}

		return 1;
	}

	cout << "Server opened on thread " << getpid() << endl;
	
	signal(SIGINT, handle_signal);
	signal(SIGABRT, handle_signal);
	signal(SIGFPE, handle_signal);
	signal(SIGILL, handle_signal);
	signal(SIGSEGV, handle_signal);
	signal(SIGTERM, handle_signal);
	signal(REQCONNECT, handle_signal);

	s_mutex.ready();
	int status = serve();

	close_server(status); // Returns 0
}

void close_server(int status) {
	cout << "Closing server." << endl;
	s_mutex.unlock();

	exit(status);
}

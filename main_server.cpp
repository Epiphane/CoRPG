#include <iostream>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "server_lock.h"

using namespace std;

ServerLock s_mutex;

void kill(int sig) {
	if (sig == SIGSEGV) {
		cerr << "Segmentation Fault" << endl;
	}

	cout << "Received signal " << sig << endl;
	if (s_mutex.is_open()) {
		cout << "Closing server" << endl;
		s_mutex.unlock();
	}

	exit(0);
}

int main(int argc, char *argv[]) {
	if (argc == 2 && strcmp(argv[1], "--force") == 0) {
		s_mutex.relink();
	}

	// Get a lock on the server
	if (!s_mutex.is_open()) {
		cout << "Blocked" << endl;

		return 1;
	}

	cout << "Server opened on thread " << getpid() << endl;

	signal(SIGINT, kill);
	signal(SIGABRT, kill);
	signal(SIGFPE, kill);
	signal(SIGILL, kill);
	signal(SIGSEGV, kill);
	signal(SIGTERM, kill);

	while(1);

	kill(SIGINT);

	return 0;
}

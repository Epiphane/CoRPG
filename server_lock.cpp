#include <iostream>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "server_lock.h"

using namespace std;

ServerLock::ServerLock() : state(SEM_UNTRIED) {
	init();
}

void ServerLock::init() {
	// Try to lock it immediately
	lock();
}

void ServerLock::relink() {
	if (is_open()) {
		unlock();
	}
	
	state = SEM_UNTRIED;
	
	init();
}

bool ServerLock::lock() {
	int res = open(LOCK, O_CREAT | O_EXCL);
	if (res >= 0) { // Gucci
		state = SEM_LOCKED;
	}
	else {
		if (errno == EEXIST) { // Blocked right now
			state = SEM_BLOCK;
		}
		else {
			cerr << "Error waiting on server lock: " << strerror(errno) << endl;
			state = SEM_UNTRIED;
		}
	}

	return is_open();
}

void ServerLock::unlock() {
	if (!is_open()) {
		cerr << "Server Lock not opened." << endl;
		return;
	}

	int res = unlink(LOCK);
	if (res) {
		cerr << "Error unlocking server: " << strerror(errno) << endl;
	}
}

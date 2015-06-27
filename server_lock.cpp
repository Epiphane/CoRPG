#include <iostream>
#include <errno.h>
#include <string.h>
#include "server_lock.h"

using namespace std;

ServerLock::ServerLock() : state(SEM_UNTRIED) {
	init();
}

void ServerLock::init() {
	server_lock = sem_open("/corpgserver", O_CREAT, 0x777, 1);
	if (server_lock == SEM_FAILED) {
		cerr << "Error opening server lock: " << strerror(errno) << endl;
	}
	
	// Try to lock it immediately
	lock();
}

void ServerLock::relink() {
	unlink();

	init();
}

void ServerLock::unlink() {
	sem_unlink("/corpgserver");
}

bool ServerLock::lock() {
	int res = sem_trywait(server_lock);
	if (res == 0) { // Gucci
		state = SEM_LOCKED;
	}
	else {
		if (errno == EAGAIN) { // Blocked right now
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

	sem_post(server_lock);
	state = SEM_UNTRIED;
}

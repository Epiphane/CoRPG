#include <iostream>
#include <errno.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

#include "client_server.h"
#include "server_lock.h"

using namespace std;

bool ServerLock::is_open() {
	return state == SEM_LOCKED;
}

bool ServerLock::lock() {
	const int mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH;
	lockfile = open(SERVER_LOCK, O_WRONLY | O_CREAT | O_EXCL, mode);
	if (lockfile >= 0) { // Gucci
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

void ServerLock::ready() {
	// Record what process locked it
	pid_t process = getpid();
	lseek(lockfile, 0, SEEK_SET);
	ssize_t res = write(lockfile, &process, sizeof(process));
	if (res == -1) {
		cerr << "Error writing to server lock: " << strerror(errno) << endl;
	}
}

void ServerLock::unlock(bool force) {
	if (!is_open() && !force) {
		cout << "Error unlocking server: not open" << endl;
		return;
	}

	int res = unlink(SERVER_LOCK);
	if (res) {
		cerr << "Error unlocking server: " << strerror(errno) << endl;
	}
	
	state = SEM_UNTRIED;
}

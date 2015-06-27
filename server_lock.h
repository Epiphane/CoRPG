#ifndef SERVER_LOCK_H
#define SERVER_LOCK_H

#include <semaphore.h>

class ServerLock {
private:
	const char* location  = "/corpgserver";
	const char* LOCK      = ".server.lock";

	const int SEM_LOCKED  = 0;
	const int SEM_BLOCK   = 1;
	const int SEM_UNTRIED = 2;

	sem_t *server_lock;
	
	int state;

	void init();
public:
	ServerLock();

	bool lock();
	void unlock();
	void relink();
	void unlink();
	bool is_open() { return state == SEM_LOCKED; }
};
 
#endif /* SERVER_LOCK_H */

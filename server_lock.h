#ifndef SERVER_LOCK_H
#define SERVER_LOCK_H

class ServerLock {
private:
	const int SEM_LOCKED  = 0;
	const int SEM_BLOCK   = 1;
	const int SEM_UNTRIED = 2;
	
	int state, lockfile;

public:
	ServerLock() : state(SEM_UNTRIED) {};

	pid_t get_pid();

	bool lock();
	void ready();
	void unlock(bool force = false);
	bool is_open();
};
 
#endif /* SERVER_LOCK_H */

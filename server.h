#ifndef SERVER_H
#define SERVER_H 

#include <sys/un.h>

class Server {
private:
	int conn;
	char name[_POSIX_PATH_MAX];
	struct sockaddr_un address;

public:
	Server(const char *name);

	bool is_open() { return conn >= 0; }

	int init(int max_connections);
	void release();

	void setBlocking(bool blocking);
	void checkConnections();
};

#endif /* SERVER_H */

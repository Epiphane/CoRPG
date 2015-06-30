#ifndef SERVER_H
#define SERVER_H 

#include <sys/un.h>
#include <vector>

#include "client_server.h"

class Server {
private:
	int conn;
	std::vector<Client> clients;

	char name[_POSIX_PATH_MAX];
	struct sockaddr_un address;

public:
	Server(const char *name);

	bool is_open() { return conn >= 0; }

	int init(int max_connections);
	void release();

	void setBlocking(bool blocking);
	Client *accept();
};

#endif /* SERVER_H */

#ifndef SERVER_H
#define SERVER_H 

#include <sys/un.h>
#include <vector>

#include "client_server.h"
#include "server_update.h"

typedef void (*reaction)(ServerUpdate *);

class Server {
private:
	int conn;
	std::vector<Client> clients;

	char name[100];
	struct sockaddr_un address;

public:
	Server(const char *name);

	bool is_open() { return conn >= 0; }

	int init(int max_connections);
	void release();

	void setBlocking(bool blocking);
	Client *accept();
	int numClients() { return clients.size(); }

	void recv_update(reaction callback);
	void send_update(const std::vector<ServerUpdate> &updates);
};

#endif /* SERVER_H */

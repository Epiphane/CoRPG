#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H 

#include <unistd.h>
#include <sys/socket.h>

#define SERVER_NAME ".server"

typedef enum { UPDATE, MESSAGE, REQUEST } UpdateType;
typedef struct ServerUpdate {
	UpdateType method;
	char data      [126];
} ServerUpdate;

/* Keeps track of existing connections */
class Client {
private:
	int conn;	

public:
	Client(int conn) : conn(conn) {};

	ssize_t send(void *buffer, size_t len, int flags);
	ssize_t send(UpdateType method, const char *data);
	ssize_t recv(void *buffer, int flags);

	void release(int flags);
	inline void close_read() { release(SHUT_RD); }
	inline void close_write() { release(SHUT_WR); }
	inline void close_rdwr() { release(SHUT_RDWR); }
};

/* Open connection to server and return socket*/
Client *connect_to_server();

#endif /* CLIENT_SERVER_H */

#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H 

#include <unistd.h>
#include <sys/socket.h>

#define SERVER_NAME ".server"

/* Open connection to server and return socket*/
int connect_to_server();

#endif /* CLIENT_SERVER_H */

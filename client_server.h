#ifndef CLIENT_SERVER_H
#define CLIENT_SERVER_H 

#include <signal.h>
#include <unistd.h>

#define SERVER_LOCK ".server.lock"
#define REQCONNECT SIGUSR1

pid_t get_server();

#endif /* CLIENT_SERVER_H */

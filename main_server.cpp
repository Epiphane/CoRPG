#include <iostream>
#include <fstream>
#include <string>
#include <signal.h>
#include <string.h>
#include <unistd.h>

#include "client_server.h"
#include "server.h"

using namespace std;

Server *server = NULL;

void close_server(int status);
void handle_signal(int sig) {
	if (sig == SIGSEGV) {
		cerr << "Segmentation Fault" << endl;
	}

	cout << "Received signal " << sig << "." << endl;;

	close_server(1);
}

int serve() {
	fstream server_state("._serverinfo_", iostream::in);
	if (!server_state.is_open()) {
		if (errno == ENOENT) {
			// Clean slate!
		}
		else {
			cerr << "Error opening ._serverinfo_: " << strerror(errno) << endl;
			return 1;
		}
	}

	bool conned = false;
	while (!conned) {
		Client *newClient = server->accept();
		if (newClient) {
			const char *msg = "Hello client!";
			ssize_t res = newClient->send((void *)msg, 14, 0);

			cout << "Sent (" << res << ") " << msg << endl;
		}
	}

	return 0;
}

int main(int argc, char *argv[]) {
	server = new Server(SERVER_NAME);
	int status = server->init(4);
	if (status == 0) {
		cout << "-------------" << endl;
		cerr << "-------------" << endl;

		signal(SIGINT, handle_signal);
		signal(SIGABRT, handle_signal);
		signal(SIGFPE, handle_signal);
		signal(SIGILL, handle_signal);
		signal(SIGSEGV, handle_signal);
		signal(SIGTERM, handle_signal);

		server->setBlocking(false);
		status = serve();
	}

	close_server(status); // Returns 0
}

void close_server(int status) {
	if (server != NULL && server->is_open()) {
		cout << "Closing server." << endl;

		server->release();
		delete server;
	}

	exit(status);
}

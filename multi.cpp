#include <iostream>
#include <errno.h>
#include <sys/file.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

int main() {
	sem_t *res = sem_open("/player1", O_CREAT | O_TRUNC, 0x777, 1);
	cout << "Grabbing" << endl;
	if (res == SEM_FAILED) {
		cout << "Error: " << strerror(errno) << endl;
	}
	else {
		if (sem_wait(res) == EAGAIN) {
			cout << "Blocked!" << endl;
		}
		cout << "Grabbed" << endl;

		unsigned long i = 0;
		while(i++ < 1000000000);
		cout << "Releasing" << endl;
		sem_post(res);
	}

	return 0;
}

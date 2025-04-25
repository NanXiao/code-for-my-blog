#include <stdio.h>
#include <pthread.h>

int
main(void)
{
	pthread_spinlock_t lock;
	if (pthread_spin_init(&lock, PTHREAD_PROCESS_PRIVATE) != 0) {
		perror("pthread_spin_init error");
		return 1;
	}

	if (pthread_spin_lock(&lock) != 0) {
		perror("pthread_spin_lock 1 error");
		return 1;
	}

	if (pthread_spin_lock(&lock) != 0) {
		perror("pthread_spin_lock 2 error");
		return 1;
	}

	return 0;
}

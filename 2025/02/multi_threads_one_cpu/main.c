#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <string.h>

#define THREAD_NUM 4
#define SUM_LOOP_SIZE 100000

uint64_t sum;

void *
thread(void *arg)
{
	for (int i = 0; i < SUM_LOOP_SIZE; i++) {
		sum++;
	}
	return NULL;
}

int
main()
{
	pthread_t tid[THREAD_NUM];
	uint64_t counter = 0;
	while (1) {
		counter++;
		for (int i = 0; i < THREAD_NUM; i++) {
			int ret = pthread_create(&tid[i], NULL, thread, NULL);
			if (ret != 0) {
				fprintf(stderr, "Create thread error: %s", strerror(ret));
				return 1;
			}
		}

		for (int i = 0; i < THREAD_NUM; i++) {
			int ret = pthread_join(tid[i], NULL);
			if (ret != 0) {
				fprintf(stderr, "Join thread error: %s", strerror(ret));
				return 1;
			}
		}

		if (sum != THREAD_NUM * SUM_LOOP_SIZE) {
			fprintf(stderr, "Exit after running %" PRIu64 " times, sum=%" PRIu64 "\n", counter, sum);
			return 1;
		}

		sum = 0;
	}

	return 0;
}

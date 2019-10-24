#include <pthread.h>
#include <stdio.h>
#include <stdatomic.h>
#include <stdint.h>

volatile uint64_t sum;
atomic_ullong atomic_sum;

void *thread(void *arg)
{
	for (int i = 0; i < 100000; i++)
	{
		sum++;
		atomic_fetch_add(&atomic_sum, 1);
	}
	return NULL;
}

int main()
{
	pthread_t tid[4];
	for (int i = 0; i < sizeof(tid) / sizeof(tid[0]); i++)
	{
		pthread_create(&tid[i], NULL, thread, NULL);
	}

	for (int i = 0; i < sizeof(tid) / sizeof(tid[0]); i++)
	{
		pthread_join(tid[i], NULL);
	}

	printf("sum=%llu,atomic_sum=%llu\n", sum, atomic_sum);
	return 0;
}


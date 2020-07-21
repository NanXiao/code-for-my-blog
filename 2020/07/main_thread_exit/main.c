#include <stdio.h>
#include <threads.h>
#include <unistd.h>

int
print_thread(void *s)
{
	thrd_detach(thrd_current());
	for (size_t i = 0; i < 5; i++)
	{
		sleep(1);
		printf("i=%zu\n", i);
	}
	thrd_exit(0);
}

int
main(void)
{
	thrd_t tid;
	if (thrd_success != thrd_create(&tid, print_thread, NULL)) {
		fprintf(stderr, "Create thread error\n");
		return 1;
	}
	thrd_exit(0);
}

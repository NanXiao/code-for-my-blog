#include <stdio.h>
#include <pthread.h>
#include <errno.h>

void *thread1_func(void *p_arg)
{
           errno = 0;
           sleep(3);
           errno = 1;
           printf("%s exit, errno is %d\n", (char*)p_arg, errno);
}

void *thread2_func(void *p_arg)
{
           errno = 0;
           sleep(5);
           printf("%s exit, errno is %d\n", (char*)p_arg, errno);
}

int main(void)
{
        pthread_t t1, t2;

        pthread_create(&t1, NULL, thread1_func, "Thread 1");
        pthread_create(&t2, NULL, thread2_func, "Thread 2");

        sleep(10);
        return;
}

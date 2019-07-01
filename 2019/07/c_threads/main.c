#include <threads.h>
#include <stdio.h>

int print_thread(void* s)
{
    printf("%s\n", (char*)s);
    thrd_exit(0);
}
int main()
{
    thrd_t tid;
    if (thrd_success != thrd_create(&tid, print_thread, "Hello world"))
    {
        fprintf(stderr, "Create thread error\n");
        return 1;
    }
    thrd_join(tid, NULL);
    return 0;
}
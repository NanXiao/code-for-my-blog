#include <stdio.h>

void foo_1(void)
{
	printf("%s:%d\n", __func__, __COUNTER__);
}

void foo_2(void)
{
	printf("%s:%d\n", __func__, __COUNTER__);
}

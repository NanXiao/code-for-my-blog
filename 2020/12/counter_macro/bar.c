#include <stdio.h>

void bar_1(void)
{
	printf("%s:%d\n", __func__, __COUNTER__);
}

void bar_2(void)
{
	printf("%s:%d\n", __func__, __COUNTER__);
}

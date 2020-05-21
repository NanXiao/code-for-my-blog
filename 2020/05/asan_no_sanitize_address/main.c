#include <stdlib.h>
#include <sanitizer/asan_interface.h>

#define ARRAY_SIZE 4

int *array;

__attribute__((no_sanitize_address))
void
foo()
{
	array[0] = 1;
}

int
main()
{
	array = malloc(sizeof(int) * 4);
	ASAN_POISON_MEMORY_REGION(array, sizeof(int) * 4);
	foo();
}

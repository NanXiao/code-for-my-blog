#include <stdalign.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

int main()
{
	printf("alignof(max_align_t)=%zu\n\n", alignof(max_align_t));

	size_t size = 1024;
	size_t align[] = {1, 2, 4, 8, 16, 32, 64};
	for (size_t i = 0; i < sizeof(align) / sizeof(align[0]); i++)
	{
		void *p = aligned_alloc(align[i], size);
		printf("align=%zu, pointer is %p\n", align[i], p);
		free(p);
	}
}


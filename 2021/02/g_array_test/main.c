#include <stdio.h>
#include <gmodule.h>

int main(void)
{
	GArray *garray = g_array_new(FALSE, FALSE, sizeof(int));
	for (gint i = 0; i < 10; i++)
	{
		g_array_append_val (garray, i);
	}
	while (garray->len != 0)
	{
		g_array_remove_index_fast(garray, 0);
	}
	printf("%d\n", garray->len);
	g_array_free(garray, TRUE);
	return 0;
}

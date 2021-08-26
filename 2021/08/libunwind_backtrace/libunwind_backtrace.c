// This code is based on https://eli.thegreenplace.net/2015/programmatic-access-to-the-call-stack-in-c/,
// so the copyright complis with the original code.

#define UNW_LOCAL_ONLY
#include <libunwind.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
// Call this function to get a backtrace.
void
backtrace(void)
{
	unw_cursor_t cursor;
	unw_context_t context;
	int ret;

	// Initialize cursor to current frame for local unwinding.
	if (unw_getcontext(&context) != 0) {
		fprintf(stderr, "unw_getcontext error!\n");
		return;
	}
	ret = unw_init_local(&cursor, &context);
	if (ret != 0) {
		fprintf(stderr, "unw_init_local error: %s", unw_strerror(ret));
		return;
	}

	int str_size = 0;
	char stack_trace[4096];
	// Unwind frames one by one, going up the frame stack.
	while (unw_step(&cursor) > 0) {
		unw_word_t offset, pc;
		ret = unw_get_reg(&cursor, UNW_REG_IP, &pc);
		if (ret != 0) {
			fprintf(stderr, "unw_get_reg error: %s", unw_strerror(ret));
			return;
		}
		if (pc == 0) {
			break;
		}
		str_size += snprintf(
		    stack_trace + str_size,
		    sizeof(stack_trace) - str_size,
		    "0x%lx:", pc);

		char sym[256];
		ret = unw_get_proc_name(&cursor, sym, sizeof(sym), &offset);
		if (ret == 0) {
			str_size += snprintf(
			    stack_trace + str_size,
			    sizeof(stack_trace) - str_size,
			    " (%s+0x%lx)\n", sym, offset);
		} else {
			str_size += snprintf(
			    stack_trace + str_size,
			    sizeof(stack_trace) - str_size,
			    " -- error(%s): unable to obtain symbol name for this frame\n", unw_strerror(ret));
		}
	}

	const char *file_name = "/tmp/backtrace.log";
	assert(pthread_mutex_lock(&mutex) == 0);
	FILE *fp = fopen(file_name, "a");
	if (fp == NULL) {
		fprintf(stderr, "Open %s failed: %s\n", strerror(errno));
		assert(pthread_mutex_unlock(&mutex) == 0);
		return;
	}
	if (fwrite(stack_trace, 1, str_size, fp) != str_size) {
		fprintf(stderr, "Write %s failed: %s\n", strerror(errno));
	}
	fclose(fp);
	assert(pthread_mutex_unlock(&mutex) == 0);
}

void
foo(void)
{
	backtrace(); // <-------- backtrace here!
}

void
bar(void)
{
	foo();
}

int
main(int argc, char **argv)
{
	bar();
	return 0;
}

CC = clang
double_lock: main.c
	$(CC) -pthread -O3 -o $@ main.c

clean:
	rm -rf double_lock

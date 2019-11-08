all: ooo copy reassemble
ooo: ooo.c
	cc ooo.c -o ooo -lpcap
copy: copy.c
	cc copy.c -o copy -lpcap
reassemble: reassemble.c
	cc reassemble.c -o reassemble -lpcap
clean:
	rm -f ooo copy reassemble

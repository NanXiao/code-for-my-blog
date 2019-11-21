all: ooo copy reassemble ipfrag
ooo: ooo.c
	cc ooo.c -o ooo -lpcap
copy: copy.c
	cc copy.c -o copy -lpcap
reassemble: reassemble.c
	cc reassemble.c -o reassemble -lpcap
ipfrag: ipfrag.c
	cc ipfrag.c -o ipfrag -lpcap
clean:
	rm -f ooo copy reassemble ipfrag

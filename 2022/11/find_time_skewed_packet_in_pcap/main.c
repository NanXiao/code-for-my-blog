#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

int
main(int argc, char **argv)
{
	if (argc != 2)
	{
		fprintf(stderr, "./main pcap_file\n");
		return EXIT_FAILURE;
	}

	const char *pcap_file = argv[1];
	int ret = EXIT_FAILURE;
	char err[PCAP_ERRBUF_SIZE];
	pcap_t *pcap_handle = pcap_open_offline(pcap_file, err);
	if (pcap_handle == NULL)
	{
		fprintf(stderr, "Open %s failed: %s\n", pcap_file, err);
		goto END;
	}

	struct pcap_pkthdr *hdr = NULL;
	const u_char *data = NULL;
	ret = pcap_next_ex(pcap_handle, &hdr, &data);
	if (ret == -1)
	{
		fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(pcap_handle));
		goto END;
	}
	size_t pkt_num = 1;
	struct timeval prev_ts = hdr->ts;

	while (1)
	{
		int ret = pcap_next_ex(pcap_handle, &hdr, &data);
		if (ret == 1)
		{
			pkt_num++;
			if (timercmp(&hdr->ts, &prev_ts, <)) {
				printf("The %zuth packet's time is screwed\n", pkt_num);
				goto END;
			}
			prev_ts = hdr->ts;
		}
		else if (ret == PCAP_ERROR_BREAK)
		{
			break;
		}
		else
		{
			fprintf(stderr, "pcap_next_ex error: %s\n", pcap_geterr(pcap_handle));
			goto END;
		}
	}
	ret = EXIT_SUCCESS;
	printf("Success, process %zu packets totally\n", pkt_num);
END:
	if (pcap_handle != NULL)
	{
		pcap_close(pcap_handle);
	}

	return ret;
}

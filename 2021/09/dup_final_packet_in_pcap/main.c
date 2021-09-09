#include <pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "./main src.pcap dst.pcap\n");
		return EXIT_FAILURE;
	}
	const char *src_file = argv[1];
	const char *dst_file = argv[2];
	int ret = EXIT_FAILURE;

	pcap_t *src_handle = NULL;
	pcap_t *dst_handle = NULL;
	pcap_dumper_t *dst_dump = NULL;

	char err[PCAP_ERRBUF_SIZE];
	src_handle = pcap_open_offline(src_file, err);
	if (src_handle == NULL)
	{
		fprintf(stderr, "Open %s failed: %s\n", src_file, err);
		goto END;
	}

	dst_handle = pcap_open_dead(DLT_EN10MB, 262144);
	dst_dump = pcap_dump_open(dst_handle, dst_file);
	if (dst_dump == NULL) 
	{
		printf("pcap_dump_open error: %s\n", pcap_geterr(dst_handle));
		return EXIT_FAILURE;
	}

	struct pcap_pkthdr hdr_backup;
	u_char data_backup[262144];

	while (1)
	{
		struct pcap_pkthdr *hdr = NULL;
		const u_char *data = NULL;
		int ret = pcap_next_ex(src_handle, &hdr, &data);
		if (ret == 1)
		{
			hdr_backup = *hdr;
			memcpy(data_backup, data, hdr->caplen);
			pcap_dump((u_char *)dst_dump, hdr, data);
		}
		else if (ret == PCAP_ERROR_BREAK)
		{
			hdr_backup.ts.tv_sec += 20;
			pcap_dump((u_char *)dst_dump, &hdr_backup, data_backup);
			break;
		}
		else
		{
			printf("pcap_next_ex error: %s\n", pcap_geterr(src_handle));
			goto END;
		}
	}
	ret = EXIT_SUCCESS;

END:
	if (dst_dump != NULL)
	{
		pcap_dump_close(dst_dump);
	}

	if (dst_handle != NULL)
	{
		pcap_close(dst_handle);
	}
	
	if (src_handle != NULL)
	{
		pcap_close(src_handle);
	}

	return ret;
}

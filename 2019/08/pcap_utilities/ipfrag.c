#include <pcap/pcap.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int
main()
{
	const char *src_file = "./unknown.pcap";
	const char *dst_file = "./ip_fragment.pcap";
	pcap_t *src_handle = NULL;
	pcap_t *dst_handle = NULL;

	char err[PCAP_ERRBUF_SIZE];
	src_handle = pcap_open_offline(src_file, err);
	if (src_handle == NULL) {
		printf("Open %s failed: %s\n", src_file, err);
		return EXIT_FAILURE;
	}

	dst_handle = pcap_open_dead(DLT_EN10MB, 262144);
	pcap_dumper_t *dst_dump = pcap_dump_open(dst_handle, dst_file);
	if (dst_dump == NULL) {
		printf("pcap_dump_open error: %s\n", pcap_geterr(dst_handle));
		return EXIT_FAILURE;
	}

	while (1)
	{
		struct pcap_pkthdr *hdr = NULL;
		const u_char *data = NULL;
		static int count = 0;

		int ret = pcap_next_ex(src_handle, &hdr, &data);
		if (ret == 1) {
			++count;
			if (count == 9) {
				uint32_t stripe_len = 20 + 8 + 12; // IP + UDP + GTP
				hdr->caplen -= stripe_len;
				hdr->len = hdr->caplen;
				u_char *new_data = calloc(1, hdr->caplen);
				memcpy(new_data, data, 14);
				memcpy(new_data + 14, data + 14 + stripe_len, hdr->caplen - 14);
				pcap_dump((u_char *)dst_dump, hdr, new_data);
				free(new_data);
			} else {
				pcap_dump((u_char *)dst_dump, hdr, data);
			}
		} else if (ret == PCAP_ERROR_BREAK) {
			break;
		} else {
			printf("pcap_next_ex error: %s\n", pcap_geterr(src_handle));
			return EXIT_FAILURE;
		}
	}

	pcap_dump_close(dst_dump);
	pcap_close(dst_handle);
	pcap_close(src_handle);

	return EXIT_SUCCESS;
}


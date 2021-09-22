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

	size_t pkt_num = 0;

	while (1)
	{
		struct pcap_pkthdr *hdr = NULL;
		const u_char *data = NULL;
		int ret = pcap_next_ex(src_handle, &hdr, &data);
		if (ret == 1)
		{
			pkt_num++;
			if (pkt_num == 5)
			{
				char csrc[] = {0x77, 0xd8, 0x36, 0xfe};
				char header_ext[] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00};
				uint16_t csrc_and_header_ext_len = sizeof(csrc) + sizeof(header_ext);

				uint16_t ethernet_header_len = 14;
				uint16_t ip_header_len = 20;
				uint16_t udp_header_len = 8;
				uint16_t rtp_fixed_header_len = 12;
				uint16_t total_header_len = ethernet_header_len + ip_header_len + udp_header_len + rtp_fixed_header_len;
				uint16_t rtp_payload_len = hdr->caplen - total_header_len;

				u_char new_data[total_header_len + csrc_and_header_ext_len + rtp_payload_len];
				memcpy(new_data, data, total_header_len);
				memcpy(new_data + total_header_len, csrc, sizeof(csrc));
				memcpy(new_data + total_header_len + sizeof(csrc), header_ext, sizeof(header_ext));
				memcpy(new_data + total_header_len + csrc_and_header_ext_len, data + total_header_len, rtp_payload_len);

				hdr->caplen += csrc_and_header_ext_len;
				hdr->len += csrc_and_header_ext_len;

				// Modify "Total Length" field in IP header
				uint16_t new_ip_packet_len = ip_header_len + udp_header_len + rtp_fixed_header_len + csrc_and_header_ext_len + rtp_payload_len;
				new_data[ethernet_header_len + 2] = (new_ip_packet_len & 0xff00) >> 8;
				new_data[ethernet_header_len + 3] = (uint8_t)new_ip_packet_len;

				// Modify "Length" field in UDP header
				uint16_t new_udp_packet_len = new_ip_packet_len - ip_header_len;
				new_data[ethernet_header_len + ip_header_len + 4] = (new_udp_packet_len & 0xff00) >> 8;
				new_data[ethernet_header_len + ip_header_len + 5] = (uint8_t)new_udp_packet_len;

				// Set Extension and "CSRC count" in RTP header
				new_data[ethernet_header_len + ip_header_len + udp_header_len] |= 0x11;
				pcap_dump((u_char *)dst_dump, hdr, new_data);
			}
			else
			{
				pcap_dump((u_char *)dst_dump, hdr, data);
			}
		}
		else if (ret == PCAP_ERROR_BREAK)
		{
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

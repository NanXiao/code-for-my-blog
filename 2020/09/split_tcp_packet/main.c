#include <pcap/pcap.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

// This header includes Ethernet + IP + TCP
uint8_t header[] = {
	// Ethernet
	0xcc, 0xf4, 0x11, 0x13, 0x98, 0x4e, 0xa4, 0x83,
	0xe7, 0xcd, 0xbb, 0x48, 0x08, 0x00,
	// IP
	0x45, 0x00, 0x02, 0x8a, 0x00, 0x00, 0x40, 0x00,
	0x40, 0x06, 0xd1, 0xad, 0xc0, 0xa8, 0x56, 0x8e,
	0x17, 0xf6, 0x37, 0x94,
	// TCP
	0xdd, 0x8d, 0x01, 0xbb, 0x1c, 0xdf, 0xfd, 0x22,
	0x35, 0x90, 0x76, 0xb8, 0x80, 0x18, 0x08, 0x07,
	0xc9, 0xe3, 0x00, 0x00, 0x01, 0x01, 0x08, 0x0a,
	0x26, 0xcb, 0xa4, 0x19, 0x01, 0x23, 0x7f, 0xc7,
};

const uint8_t tcp_payload_1[] = {
	0x17, 0x03, 0x03, 
};
const uint8_t tcp_payload_2[] = {
	0x02,
};
const uint8_t tcp_payload_3[] = {
	0x51, 0x39, 0xcf, 0x58,
	0x32, 0xac, 0xfc, 0x07, 0xfb, 0x8b, 0x34, 0xca,
	0x69, 0xa4, 0x93, 0x07, 0x3b, 0x05, 0xde, 0x14,
	0x41, 0xdf, 0xcc, 0x1d, 0x3e, 0x69, 0xd9, 0xf8,
	0x8d, 0x7c, 0xb1, 0x53, 0xec, 0x01, 0x7b, 0xef,
	0xc2, 0xcf, 0x8b, 0x57, 0xe8, 0x2b, 0xdb, 0xe8,
	0xca, 0x53, 0x11, 0xe6, 0x95, 0xa2, 0xb0, 0x6a,
	0xcd, 0x4e, 0xbd, 0x3b, 0xe1, 0x2d, 0xfa, 0x16,
	0x4c, 0x42, 0x07, 0x3d, 0xe2, 0x85, 0xae, 0x7e,
	0x4f, 0x17, 0x46, 0x2d, 0x1d, 0xdf, 0xc8, 0x35,
	0xa7, 0x29, 0xc0, 0xbf, 0xc5, 0xfd, 0x55, 0x77,
	0x96, 0xc6, 0xfa, 0xa3, 0xbc, 0x7e, 0xcc, 0x7a,
	0xf3, 0xb2, 0xde, 0x81, 0xfd, 0x1d, 0x46, 0x7a,
	0x87, 0x1c, 0x5c, 0xb9, 0x13, 0x5a, 0xe7, 0x1c,
	0xc3, 0xba, 0x93, 0x44, 0x7a, 0x19, 0xb0, 0xe2,
	0xc2, 0x7e, 0xe7, 0x6c, 0x02, 0x80, 0xb0, 0x8d,
	0x33, 0xcb, 0x2c, 0x85, 0x78, 0xf8, 0x0f, 0xa4,
	0x82, 0xaf, 0x3c, 0xeb, 0x63, 0xf4, 0x52, 0xc4,
	0xdf, 0xc1, 0x7e, 0x20, 0x5d, 0x07, 0x41, 0x56,
	0x9c, 0x31, 0x32, 0xbc, 0xd2, 0x73, 0xe3, 0x6e,
	0x74, 0x23, 0xe8, 0x20, 0x47, 0xdf, 0xa4, 0x12,
	0x68, 0xd5, 0xca, 0x7d, 0x96, 0xe7, 0x90, 0x29,
	0x1d, 0xf8, 0x20, 0x53, 0x6f, 0xc4, 0xde, 0x96,
	0xf5, 0xef, 0x0a, 0x0a, 0x71, 0x3e, 0x4e, 0x91,
	0x1e, 0x16, 0x8f, 0xfd, 0xf6, 0xd2, 0x10, 0x04,
	0x18, 0x21, 0xb0, 0xec, 0x57, 0xf3, 0xa1, 0x13,
	0xf2, 0x71, 0x83, 0xe2, 0x42, 0xe1, 0x87, 0x37,
	0xc8, 0xb8, 0x43, 0x5d, 0xfb, 0x84, 0x14, 0xc4,
	0xf0, 0xf7, 0xc1, 0x28, 0x49, 0x53, 0x67, 0x0b,
	0x0e, 0xf2, 0xba, 0xab, 0xc4, 0x35, 0x13, 0x32,
	0x32, 0x57, 0x82, 0xd7, 0x94, 0xea, 0x16, 0x37,
	0xc8, 0x5b, 0xa9, 0xcb, 0xbe, 0xb3, 0x8e, 0x79,
	0x09, 0x89, 0x47, 0x9b, 0x36, 0xd6, 0x92, 0xcf,
	0x75, 0xf8, 0xe4, 0x79, 0x33, 0x28, 0xfb, 0x91,
	0x9c, 0xaa, 0x9a, 0xdc, 0x9e, 0x6a, 0x2e, 0xf3,
	0x11, 0x23, 0x20, 0xc3, 0x0a, 0xe4, 0x97, 0x65,
	0x34, 0x4b, 0xb8, 0x02, 0x55, 0x51, 0x21, 0x3f,
	0xcd, 0x0e, 0x0f, 0x12, 0xab, 0x1e, 0x6d, 0x7f,
	0x85, 0xca, 0x78, 0x17, 0x5b, 0x53, 0xc3, 0xbc,
	0xfe, 0x87, 0x8b, 0xe7, 0xe7, 0xe5, 0xf6, 0x13,
	0xc8, 0xee, 0x35, 0x5f, 0x20, 0x16, 0x16, 0x43,
	0xf1, 0x40, 0x4e, 0x3f, 0xe8, 0xb3, 0x88, 0x0a,
	0x4e, 0xb6, 0x10, 0xad, 0xcd, 0xe8, 0x52, 0xd7,
	0x63, 0x5e, 0x3c, 0x91, 0x30, 0xd1, 0x97, 0x9e,
	0x5a, 0xca, 0xb5, 0xf8, 0x82, 0x47, 0xed, 0xfa,
	0x22, 0xec, 0x66, 0xfd, 0x5a, 0x42, 0xde, 0xce,
	0x89, 0xf5, 0xff, 0xde, 0xff, 0x70, 0x5a, 0x48,
	0x01, 0xdb, 0xc2, 0x94, 0xcf, 0x9a, 0x22, 0x11,
	0x17, 0x9b, 0x80, 0xb4, 0x29, 0xcb, 0x9f, 0xa6,
	0x5f, 0x71, 0x3d, 0x2d, 0x43, 0x02, 0x1a, 0x1c,
	0x46, 0x34, 0x6a, 0xd2, 0x4d, 0x7d, 0x1c, 0xf3,
	0x56, 0x5f, 0x86, 0xcd, 0x23, 0x89, 0x0a, 0x22,
	0xd7, 0xa1, 0x87, 0x5e, 0x0f, 0xe1, 0x9c, 0x9b,
	0xb7, 0x6a, 0x49, 0x78, 0xa1, 0x85, 0x28, 0x2f,
	0x0f, 0xaf, 0x26, 0x0c, 0x5a, 0x4b, 0x64, 0x96,
	0x5e, 0xf1, 0x67, 0xe8, 0x89, 0x92, 0xcb, 0x7f,
	0xbd, 0x1c, 0x78, 0x16, 0x09, 0xab, 0x42, 0x12,
	0x23, 0xbe, 0x1f, 0xbb, 0xcc, 0x5d, 0x1b, 0xc3,
	0xc6, 0xc7, 0xc0, 0x86, 0x4f, 0x75, 0xae, 0x63,
	0xd4, 0xae, 0x4d, 0xe9, 0x59, 0x8d, 0x47, 0xed,
	0x7c, 0x66, 0x9f, 0xc9, 0x0e, 0x1e, 0xbd, 0xc2,
	0x8e, 0x85, 0xd2, 0x3b, 0xa4, 0x29, 0x1e, 0x8a,
	0x01, 0xc6, 0x21, 0xa6, 0xd7, 0x9a, 0x31, 0x28,
	0xf6, 0x95, 0x36, 0x89, 0x45, 0x57, 0xc4, 0x13,
	0x0a, 0x8f, 0xac, 0x2f, 0xcb, 0x16, 0x1c, 0x27,
	0x7a, 0x25, 0xc6, 0xcd, 0xfb, 0x40, 0x8f, 0x04,
	0x91, 0x62, 0x96, 0x8e, 0x90, 0x93, 0x76, 0x96,
	0x0a, 0x75, 0x53, 0x60, 0xff, 0x2e, 0x0f, 0xef,
	0xd0, 0x71, 0xbe, 0x48, 0xee, 0x57, 0xc7, 0x5c,
	0x8f, 0x3c, 0xd3, 0x6d, 0x21, 0xf0, 0x8c, 0x34,
	0x30, 0x22, 0xd1, 0x81, 0xc6, 0xf8, 0xf7, 0x9e,
	0x2a, 0x49, 0x65, 0x0f, 0x82, 0x1f, 0x71, 0xc7,
	0x47, 0x33, 0x8d, 0xe4, 0xaa, 0xf8, 0xe3, 0xe9,
	0xa1, 0xee, 0x2b, 0xb4, 0xe8, 0x3c, 0xeb, 0xe4,
	0x68, 0x28, 0x1a, 0xd2, 0xa4, 0x37,
};

uint16_t ethernet_header_len = 14;
uint16_t ip_header_len = 20;
uint16_t tcp_header_len = 32;
uint32_t seq_num = 0x1cdffce2;

void dump_tcp_packet(pcap_dumper_t *pcap_dumper, struct pcap_pkthdr *pcap_header, const uint8_t *payload, uint16_t payload_size)
{
	// Modify "Total Length" field in IP header
	uint16_t ip_packet_len = ip_header_len + tcp_header_len + payload_size;
	header[ethernet_header_len + 2] = (ip_packet_len & 0xff00) >> 8;
	header[ethernet_header_len + 3] = (uint8_t)ip_packet_len;

	// Modify "Sequence number" field in TCP header
	*(uint32_t*)&header[ethernet_header_len + ip_header_len + 4] = htonl(seq_num);
	seq_num += payload_size;

	pcap_header->len = sizeof(header) + payload_size;
	pcap_header->caplen = pcap_header->len;

	uint8_t pcap_data[pcap_header->len];
	memcpy(pcap_data, header, sizeof(header));
	memcpy(&pcap_data[sizeof(header)], payload, payload_size);

	pcap_dump((u_char *)pcap_dumper, pcap_header, pcap_data);
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		fprintf(stderr, "Usage: ./main src_pcap dst_pcap\n");
		return EXIT_FAILURE;
	}

	const char *src_file = argv[1];
	const char *dst_file = argv[2];
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
		struct pcap_pkthdr *pcap_header = NULL;
		const u_char *data = NULL;
		static int count = 0;

		int ret = pcap_next_ex(src_handle, &pcap_header, &data);
		if (ret == 1) {
			++count;
			if (count == 7) {
				uint16_t payload_len = pcap_header->caplen - sizeof(header);
				uint8_t payload[payload_len + sizeof(tcp_payload_1)];
				memcpy(payload, &data[sizeof(header)], payload_len);
				memcpy(&payload[payload_len], tcp_payload_1, sizeof(tcp_payload_1));
				dump_tcp_packet(dst_dump, pcap_header, payload, payload_len + sizeof(tcp_payload_1));
			} else if (count == 8) {
				dump_tcp_packet(dst_dump, pcap_header, tcp_payload_2, sizeof(tcp_payload_2));
				dump_tcp_packet(dst_dump, pcap_header, tcp_payload_3, sizeof(tcp_payload_3));
			} else {
				pcap_dump((u_char *)dst_dump, pcap_header, data);
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

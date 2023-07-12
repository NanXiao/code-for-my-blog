#include <iostream>
#include <PcapFileDevice.h>

int main(int argc, char* argv[])
{
	if (argc != 3) {
		std::cerr << "Usage: ./convert_pcapng_to_pcap /path/from/a.pcapng /path/to/b.pcap" << std::endl;
		return 1;
	}

	// create a pcapng file reader
	pcpp::PcapNgFileReaderDevice pcapNgReader(argv[1]);
	pcapNgReader.open();

	// create a pcap file writer
	pcpp::PcapFileWriterDevice pcapWriter(argv[2]);
	pcapWriter.open();

	// raw packet object
	pcpp::RawPacket rawPacket;

	// read packets from pcap reader and write pcapng writer
	while (pcapNgReader.getNextPacket(rawPacket)) {
  		pcapWriter.writePacket(rawPacket);
	}

	return 0;
}


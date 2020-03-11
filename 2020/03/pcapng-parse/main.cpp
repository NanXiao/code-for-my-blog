#include <iostream>
#include <PcapFileDevice.h>

int
main()
{
	std::cout << pcap_lib_version() << '\n';
	pcpp::PcapNgFileReaderDevice input_file("/Users/nanxiao/Downloads/capture.pcapng");
	if (input_file.open()) {
		std::cout << "Open successfully\n";
	} else {
		std::cerr << "Open failed\n";
		return 1;
	}

	std::cout << input_file.getOS() << '\n';
	std::cout << input_file.getHardware() << '\n';
	std::cout << input_file.getCaptureApplication() << '\n';
	std::cout << input_file.getCaptureFileComment() << '\n';

	pcpp::RawPacket packet;
	std::string comment;
	for (size_t i = 1; i < 10; i++)
	{
		if (input_file.getNextPacket(packet, comment)) {
			std::cout << i << ":" << comment << '\n';
		} else {
			std::cerr << "Get packet failed\n";
			return 1;
		}
	}

}

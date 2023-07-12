include /opt/homebrew/Cellar/pcapplusplus/22.11/etc/PcapPlusPlus.mk

# All Target
all:
	g++ $(PCAPPP_BUILD_FLAGS) $(PCAPPP_INCLUDES) -c -o main.o main.cpp
	g++ $(PCAPPP_LIBS_DIR) -o convert_pcapng_to_pcap main.o $(PCAPPP_LIBS)

# Clean Target
clean:
	rm main.o
	rm convert_pcapng_to_pcap

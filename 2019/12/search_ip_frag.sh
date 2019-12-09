#!/bin/sh

for file in ./*.pcap
do
	frag_packets=$(tshark -r $file -Y "ip.flags.mf==1 || ip.frag_offset>0")
	if [[ "${frag_packets}" != "" ]]
	then
		echo "$file"
	fi
done

#!/bin/sh

directory=./pcap

cd "$directory" || exit 1

for old_file_name in *
do
	timestamp=$(tshark -nr "${old_file_name}" -T fields -e frame.time_epoch -c 1)
	new_file_name="${old_file_name}.${timestamp}.pcap"
	mv "${old_file_name}" "${new_file_name}"
done

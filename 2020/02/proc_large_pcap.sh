#!/bin/sh

input_pcap=input.pcap
output_pcap=./pcap/frag.pcap
spilt_size=1000
output_index=1
loop_count=10
exit_flag=0

command() {
	echo "$1" "$2" > log"$2"
}

tcpdump -r ${input_pcap} -w ${output_pcap} -C ${spilt_size}

command ${output_pcap}

while :
do
	loop_index=0
	while test ${loop_index} -lt ${loop_count}
	do
		if test -e ${output_pcap}${output_index}
		then
			command ${output_pcap} ${output_index} &
			output_index=$((output_index + 1))
			loop_index=$((loop_index + 1))
		else
			exit_flag=1
			break
		fi
	done
	wait

	if test ${exit_flag} -eq 1
	then
		exit 0
	fi
done

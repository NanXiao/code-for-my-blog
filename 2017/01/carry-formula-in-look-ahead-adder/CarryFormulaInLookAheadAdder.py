#!/usr/bin/python3

import sys
num = 8

if len(sys.argv) != 1:
	num = int(sys.argv[1])
	
dict = {'C0': 'C0'}
for i in range(1, num + 1):
	dict['C' + str(i)] = 'G' + str(i - 1) + \
						'+' + \
						'+'.join([x + "*P" + str(i - 1) for x in dict['C' + str(i - 1)].split('+')])

for i in range(0, num + 1):
	key = 'C' + str(i)
	print(key, "=", dict[key])

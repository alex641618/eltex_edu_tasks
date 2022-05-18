#!/bin/bash

count=0
if [ $1 ]; then
	while [ $count -ne $1 ] 
	do
		./client -a 127.0.0.1 -p 7 &
		((count = count + 1))
		sleep 0.01 
	done

read

killall -2 ./client
fi
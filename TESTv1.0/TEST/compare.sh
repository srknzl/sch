#!/bin/bash
i=1
key="/key.txt"
out="/output.txt"
while [ $i -le 10 ]
do
	if cmp -s "./test$i$key" "./test$i$out" 
	then
	echo "test$i : OK"
	else 
	echo "test$i : NOT OK"
	fi
	((i++))
done
echo "Copyright © enesozcan"
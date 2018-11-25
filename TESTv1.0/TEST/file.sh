#!/bin/bash
i=1
while [ $i -le 10 ]
do
	cd "test$i"
	"./a.out"
	cd ".." 
	((i++))
done
echo "Copyright © serkanozel"

#!/usr/bin/env bash

num_of_tests=$(ls ./tests| wc -l)
for((i=0;i<num_of_tests;i++))
do
	ARGS[i]=$(cat ./tests/test_"$i".txt)
done

sum=0
sum_iter=0
AVERAGEITER=10

for((i=0;i<num_of_tests;i++))
do
	ARG="${ARGS[i]}"
	for ((num_of_threads=1;num_of_threads<10;num_of_threads++))
	do
		for ((j=0;j<AVERAGEITER;j++))
		do
			addthis=$(echo "$ARG" "$num_of_threads" | ./build/os_lab_03)
			sum=$(bc <<< "scale=10;$sum+($addthis/$AVERAGEITER)") #why can't I use _ with bc?
		done
		sums[sum_iter]=$sum
		sum_iter=$((sum_iter+1))
		sum=0
	done

	echo "Average result for" "$i" "test"

	for ((j=0;j<9;j++))
	do
		echo -n $((j+1)) "thread: "
		echo "${sums[j]}"
		sums[j]=0
	done
	sum_iter=0
done

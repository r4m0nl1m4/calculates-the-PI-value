#!/bin/bash

# To run on terminal: ./shellscript_start.sh

#remove temp. files
rm result_report-parallel-cpu.txt
rm result_report-serie.txt
rm result_report-parallel.txt
rm calculates-the-pi-value-serial
rm calculates-the-pi-value-parallel

#serie compiler
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o calculates-the-pi-value-serial calculates-the-pi-value-serial.c
#parallel compiler
mpicc -g -Wall calculates-the-pi-value-parallel.c -o calculates-the-pi-value-parallel -lm

echo -e "\n/* \n * CPU Report \n */" >> "result_report-parallel-cpu.txt"
echo -e "\n /* CPU */ \n">> "result_report-parallel-cpu.txt"
cat /proc/cpuinfo | grep 'model name' | uniq >> "result_report-parallel-cpu.txt"
cat /proc/cpuinfo | grep 'vendor' | uniq >> "result_report-parallel-cpu.txt"
cat /proc/cpuinfo | grep 'cpu cores' | uniq >> "result_report-parallel-cpu.txt"
cat /proc/cpuinfo | grep 'siblings' | uniq >> "result_report-parallel-cpu.txt"
cat /proc/cpuinfo | grep 'cache size' | uniq >> "result_report-parallel-cpu.txt"
echo -e "\n /* Calculation Reports */ " >> "result_report-parallel-cpu.txt"

echo -e "\n/* \n * Result Report Serie \n */" >> "result_report-serie.txt"
echo -e "\n/* \n * Result Report Parallel \n */" >> "result_report-parallel.txt"

#attempts by number of cores and size
attempts=5
for cores in 2 4 6 8
do 
	for sizeProblem in 800 1600
	do 
		echo -e "\n\t$cores\t$sizeProblem\c" >> "result_report-serie.txt"
		echo -e "\n\t$cores\t$sizeProblem\c" >> "result_report-parallel.txt"
	    echo -e "\n $cores Cores CPU - Size Problem $sizeProblem \n" >> "result_report-parallel-cpu.txt"
		for attempt in $(seq $attempts)
		do
			echo -e "  Try $attempt" >> "result_report-parallel-cpu.txt"
			#serie execute
			./calculates-the-pi-value-serial $sizeProblem
			#parallel execute
			mpirun -np $cores --oversubscribe ./calculates-the-pi-value-parallel $sizeProblem
		done 
	done
done
exit
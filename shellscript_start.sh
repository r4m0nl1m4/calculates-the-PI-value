#!/bin/bash

# To run on terminal: ./shellscript_start.sh

#remove temp. files
rm result_report-serie-runtime.txt
rm result_report-parallel-runtime.txt
rm result_report-parallel-cpu.txt
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

echo -e "\n/* \n * Runtime Report In Seconds\n */" >> "result_report-serie-runtime.txt"
echo -e "\n/* \n * Runtime Report In Seconds\n */" >> "result_report-parallel-runtime.txt"

#attempts by number of cores and size
attempts=5
for cores in 2 4 6 8
do 
	for sizeProblem in 20000 40000
	do 
		echo -e "\n $cores $sizeProblem\t\c" >> "result_report-serie-runtime.txt"
		echo -e "\n $cores $sizeProblem\t\c" >> "result_report-parallel-runtime.txt"
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

#txt2pdf
vim result_report-serie-runtime.txt -c "hardcopy > serie-runtime.ps | q"; ps2pdf serie-runtime.ps; rm serie-runtime.ps
vim result_report-parallel-runtime.txt -c "hardcopy > parallel-runtime.ps | q"; ps2pdf parallel-runtime.ps; rm parallel-runtime.ps
vim result_report-parallel-cpu.txt -c "hardcopy > parallel-cpu.ps | q"; ps2pdf parallel-cpu.ps; rm parallel-cpu.ps
pdfunite serie-runtime.pdf parallel-runtime.pdf parallel-cpu.pdf report.pdf; rm serie-runtime.pdf; rm parallel-runtime.pdf; rm parallel-cpu.pdf

exit
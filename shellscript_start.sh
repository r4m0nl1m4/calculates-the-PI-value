#!/bin/bash

# To run on terminal: ./shellscript_start.sh

fileName1="result_report-cpu.txt"
fileName2="result_report-serie-runtime.txt"
fileName3="result_report-parallel-runtime.txt"
fileName4="result_report-speedup.txt"
fileName5="result_report-parallel-cpu.txt"

fileHeader1="\n/* \n * CPU Report                         \n */\n"
fileHeader2="\n/* \n * Serie Runtime Report In Seconds    \n */  "
fileHeader3="\n/* \n * Parallel Runtime Report In Seconds \n */  "
fileHeader4="\n/* \n * Speedup Report In %                \n */  "
fileHeader5="\n/* \n * Parallel Calculation Report        \n */  "

echo -e "$fileHeader1" >> $fileName1
echo -e "$fileHeader2" >> $fileName2
echo -e "$fileHeader3" >> $fileName3
echo -e "$fileHeader4" >> $fileName4
echo -e "$fileHeader5" >> $fileName5

#serie compiler
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o calculates-the-pi-value-serial calculates-the-pi-value-serial.cpp
#parallel compiler
mpic++ -g -Wall calculates-the-pi-value-parallel.cpp -o calculates-the-pi-value-parallel -lm
#analysis compiler
g++ -O0 -g -W -ansi -pedantic -std=c++11 -o calculates-serie-parallel-analysis calculates-serie-parallel-analysis.cpp

insertCPUInfo(){
	cat /proc/cpuinfo | grep "$2" | uniq >> $1	
}
insertCPUInfo $fileName1 'model name'
insertCPUInfo $fileName1 'vendor'
insertCPUInfo $fileName1 'cpu cores'
insertCPUInfo $fileName1 'siblings'
insertCPUInfo $fileName1 'cache size'

#attempts by number of cores and size
attempts=5
for cores in 2 4 6 8
do 
	for sizeProblem in 20000 40000
	do 
		echo -e "\n $cores $sizeProblem\t\c                        " >> $fileName2
		echo -e "\n $cores $sizeProblem\t\c                        " >> $fileName3
		echo -e "\n $cores $sizeProblem\t\c                        " >> $fileName4
	    echo -e "\n $cores Cores CPU - Size Problem $sizeProblem \n" >> $fileName5
		for attempt in $(seq $attempts)
		do
			echo -e "  Try $attempt" >> $fileName5
			#serie execute
			./calculates-the-pi-value-serial $sizeProblem
			#parallel execute
			mpirun -np $cores --oversubscribe ./calculates-the-pi-value-parallel $sizeProblem
			#analysis execute
            ./calculates-serie-parallel-analysis
		done
	done
done

showOnTerminal(){
	while IFS= read -r line
	do
	    echo "$line"
	done <"$1"
}
showOnTerminal $fileName1
showOnTerminal $fileName2
showOnTerminal $fileName3
showOnTerminal $fileName4
echo -e

txt2pdf(){
    vim $1 -c "hardcopy > $1.ps | q";ps2pdf $1.ps; rm $1.ps
}
txt2pdf $fileName1
txt2pdf $fileName2
txt2pdf $fileName3
txt2pdf $fileName4
txt2pdf $fileName5

pdfunite $fileName1.pdf $fileName2.pdf $fileName3.pdf $fileName4.pdf $fileName5.pdf report.pdf

rm $fileName1.pdf $fileName2.pdf $fileName3.pdf $fileName4.pdf $fileName5.pdf
rm $fileName1 $fileName2 $fileName3 $fileName4 $fileName5
rm calculates-the-pi-value-serial
rm calculates-the-pi-value-parallel
rm calculates-serie-parallel-analysis

exit
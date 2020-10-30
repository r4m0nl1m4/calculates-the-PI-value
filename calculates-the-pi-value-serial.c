
//Main for "calculates-pi-value-serial" C application
//Created by r4m0nl1m4 14/10/2020

//library(ies)
#include <stdlib.h>

//new library(ies)
#include "./pi.h"
#include "./report.h"

int main(int argc, char** argv){

    /* Allocate serie environment variables */
    double problemSize, pi, executeTime;
    struct timeval timeStart, timeEnd;
    
    /* Set serie environment variables */
    problemSize = atoi(argv[1]);

    gettimeofday(&timeStart, 0);

    pi = getPIValueByIntegration(problemSize);

    gettimeofday(&timeEnd, 0);

    executeTime = getExecuteTime(timeStart, timeEnd);  

    saveResultReportOnFile("result_report-serie.txt", pi, executeTime);

    return 0;
}

//Main for "calculates-pi-value-serial" C++ application
//Created by r4m0nl1m4 14/10/2020

#include "./pi.h"
#include "./report.h"

int main(int argc, char** argv){

    double problemSize, pi, executeTime;
    struct timeval timeStart, timeEnd;
    
    problemSize = atoi(argv[1]);

    gettimeofday(&timeStart, 0);

    pi = getPIValueByIntegration(problemSize);

    gettimeofday(&timeEnd, 0);

    executeTime = getExecuteTime(timeStart, timeEnd);  

    saveResultReportOnFile("result_report-serie-runtime.txt", executeTime);

    return 0;
}
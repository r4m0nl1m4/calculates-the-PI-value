
//Main for "calculates-speedup" C application
//Created by r4m0nl1m4 14/10/2020

//library(ies)
#include <stdlib.h>

//new library(ies)
#include "./report.h"

int main(int argc, char** argv){

    saveSpeedupReportOnFile("result_report-speedup.txt", "result_report-serie-runtime.txt", "result_report-parallel-runtime.txt");

    return 0;
}
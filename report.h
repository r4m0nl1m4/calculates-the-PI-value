
//Main for "report" C application
//Created by r4m0nl1m4 14/10/2020

//Guard
#ifndef REPORT_H
#define REPORT_H

//library(ies)
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>

double getExecuteTime(struct timeval start, struct timeval end){
    double executeTime = (double)(end.tv_usec - start.tv_usec) / 1000000 + (double)(end.tv_sec - start.tv_sec);
    return executeTime;
}

void saveResultReportOnFile(const char* fileName, double value, double executeTime){
    FILE *fp = fopen(fileName, "a");  
    bool fileOpened = (fp != NULL) ? true : false;  
    if(fileOpened)
    {
        fprintf(fp, "\t%lf  %1.2e [s] ", value, executeTime);
        fclose(fp);
    }
    else{
        fprintf(stderr, "Can't open output file %s!\n", fileName);
        exit(1);        
    }
}

void saveCPUReportOnFile(const char* fileName, int numtasks, int problemSize, double local[], double total[], double executeTime){
    FILE *fp = fopen(fileName, "a");  
    bool fileOpened = (fp != NULL) ? true : false;  
    if(fileOpened)
    {
        fprintf(fp, "\n  runtime %1.2e [s] \n", executeTime);
        fprintf(fp, "\n  Process      Total           problemSize    Total           LocalResult            CurrentResult\n");
        for( int process=0; process<numtasks; process++ )
            fprintf(fp, "  %d\t\t%d\t\t%d\t\t%d\t\t%lf\t\t%lf\n", process, numtasks, problemSize/numtasks, problemSize, local[process], total[process]);
        fprintf(fp, "--------------------------------------------------------------------------------------------------\n");
        fclose(fp);
    }
    else{
        fprintf(stderr, "Can't open output file %s!\n", fileName);
        exit(1);        
    }
}

#endif
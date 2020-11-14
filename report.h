
//Main for "report" C application
//Created by r4m0nl1m4 14/10/2020

//Guard
#ifndef REPORT_H
#define REPORT_H

//library(ies)
#include <sys/time.h>
#include <stdbool.h>
#include <stdio.h>

/*
 * Data Equations
 */

double getExecuteTime(struct timeval start, struct timeval end){
    double seconds, microseconds, executeTime;
    seconds = (double)(end.tv_sec - start.tv_sec);
    microseconds = (double)(end.tv_usec - start.tv_usec);
    executeTime = seconds + microseconds / 1000000;
    return executeTime;
}

double getSpeedup(double timeSerie, double timeParallel){
    double speedup;
    speedup = ( timeSerie / timeParallel );
    return speedup;
}

/*
 * Data Search
 */

int getDataLine(const char* fileName1, const char* fileName2){
    FILE *fp1 = fopen(fileName1, "r");
    FILE *fp2 = fopen(fileName2, "r");
    bool fileOpened1 = (fp1 != NULL) ? true : false;
    bool fileOpened2 = (fp2 != NULL) ? true : false; 
    if (fileOpened1 && fileOpened2){
        char line1[256], line2[256];
        int lineSize, lineCount, dataLine;
        lineSize = sizeof line1;
        lineCount = 1;
        for(int i=0; i<lineSize; i++) line1[i]='\0';
        for(int i=0; i<lineSize; i++) line2[i]='\0';
        while ((fgets(line1, lineSize, fp1) != NULL)&&(fgets(line2, lineSize, fp2) != NULL)) lineCount++;
        fclose(fp1);
        fclose(fp2);
        dataLine = lineCount-1;
        return dataLine;
    }
    else{
        fprintf(stderr, "Can't open output files %s and %s!\n", fileName1, fileName2);
        exit(1);  
    }
    return 0;
}

int getDataColumn(const char* fileName1, const char* fileName2, int dataLine){
    FILE *fp1 = fopen(fileName1, "r");
    FILE *fp2 = fopen(fileName2, "r");
    bool fileOpened1 = (fp1 != NULL) ? true : false;
    bool fileOpened2 = (fp2 != NULL) ? true : false; 
    if (fileOpened1 && fileOpened2){
        char line1[256], line2[256];
        int lineSize, lineCount, matchCount, dataColumn;
        lineSize = sizeof line1;
        lineCount = 1;
        matchCount = 0;
        for(int i=0; i<lineSize; i++){
            line1[i]='\0';
            line2[i]='\0';
        }
        while ((fgets(line1, lineSize, fp1) != NULL)&&(fgets(line2, lineSize, fp2) != NULL))
        {
            if (lineCount == dataLine){
                //printf("dataline[%d] = %s \n", dataLine, line1);
                bool dataMatch;
                for(int i=0; i<lineSize; i++){
                    //printf("line1[%d] == %c \n", i, line1[i]);
                    dataMatch = (line1[i] == ':') ? true : false;
                    if(dataMatch) {
                        matchCount++;
                        dataColumn=i;
                        //printf("\n\ndataline[%d][%d] = %c match = %d\n\n", dataLine, i, line1[i], matchCount);
                    }
                }
            }
            for(int i=0; i<lineSize; i++){
                line1[i]='\0';
                line2[i]='\0';
            }
            lineCount++;
        }

        fclose(fp1);
        fclose(fp2);

        return dataColumn;
    }
    else
    {
        fprintf(stderr, "Can't open output files %s and %s!\n", fileName1, fileName2);
        exit(1);  
    }
    return 0;
}

int getDataSize(char *line, int lineSize, int start){
    int end, dataSize;
    for(int i=start+1; i<lineSize; i++){
        if(line[i] == ':' || line[i] == '\n' || line[i] == '\0'){
            end = i-1;
            dataSize = end-start;
            return dataSize;
        }
    }
    return 0;
}

void getData(char *line, char *data, int dataColumn){
    int dataSize = sizeof data;
    int start = dataColumn+1;
    for(int i=0; i<dataSize; i++){
        if (i==dataSize-1){
            data[i] = line[start+i];
            data[i+1] = '\0';                                
        }
        else
            data[i] = line[start+i];
    }
}

/*
 * Get Data By File(s)
 */

double getSpeedupByFiles(const char* fileName1, const char* fileName2){
    int dataLine = getDataLine(fileName1, fileName2);
    int dataColumn = getDataColumn(fileName1, fileName2, dataLine);
    //printf("line = %d column = %d", dataLine, dataColumn);
    FILE *fp1, *fp2;
    bool fileOpened1, fileOpened2;
    fp1 = fopen(fileName1, "r");
    fp2 = fopen(fileName2, "r");
    fileOpened1 = (fp1 != NULL) ? true : false;
    fileOpened2 = (fp2 != NULL) ? true : false; 
    if (fileOpened1 && fileOpened2){
        char line1[256], line2[256];
        int lineSize, lineCount;
        lineSize = sizeof line1;
        lineCount = 1;
        for(int i=0; i<lineSize; i++){
            line1[i]='\0';
            line2[i]='\0';
        }
        while ((fgets(line1, lineSize, fp1) != NULL)&&(fgets(line2, lineSize, fp2) != NULL)){
            //printf("%s", &line);
            if (lineCount == dataLine){
                int dataSize1 = getDataSize(line1, lineSize, dataColumn);
                int dataSize2 = getDataSize(line2, lineSize, dataColumn);
                //printf("\n serieSize = %d, parallelSize = %d", dataSize1, dataSize2);
                char *data1 = new char[dataSize1+1];
                char *data2 = new char[dataSize2+1];
                getData(line1, data1, dataColumn);
                getData(line2, data2, dataColumn);
                //printf("\n serieData = %s, parallelData = %s", data1, data2);
                double timeSerie, timeParallel, speedup;
                timeSerie = atof(data1);
                timeParallel = atof(data2);
                speedup = getSpeedup(timeSerie, timeParallel);
                //printf("\n timeSerie = %1.2e, timeParallel = %1.2e, speedup = %.2f\n", timeSerie, timeParallel, speedup);
                delete[] data1;
                delete[] data2;
                return speedup;
            }
            lineCount++;
        }
    }
    else{
        fprintf(stderr, "Can't open output files %s and %s!\n", fileName1, fileName2);
        exit(1);  
    }
    return 0;
}

/*
 * Save On File
 */

void saveDoubleOnFile(const char* fileName, double value){
    FILE *fp = fopen(fileName, "a");  
    bool fileOpened = (fp != NULL) ? true : false;  
    if(fileOpened){
        fprintf(fp, ":%.2e", value);
        fclose(fp);
    }
    else{
        fprintf(stderr, "Can't open output file %s!\n", fileName);
        exit(1);        
    }
}

void saveResultReportOnFile(const char* fileName, double executeTime){
    saveDoubleOnFile(fileName, executeTime);
}

void saveSpeedupReportOnFile(const char* fileName0, const char* fileName1, const char* fileName2){
    double speedup = getSpeedupByFiles(fileName1, fileName2);
    saveDoubleOnFile(fileName0, speedup);
}

void saveCPUReportOnFile(const char* fileName, int numtasks, int problemSize, double local[], double total[], double executeTime){
    FILE *fp = fopen(fileName, "a");  
    bool fileOpened = (fp != NULL) ? true : false;  
    if(fileOpened){
        fprintf(fp, "\n    Runtime %1.2e [s] \n", executeTime);
        fprintf(fp, "\n    Process        Total  problemSize        Total        Local        Total\n");
        for( int process=0; process<numtasks; process++ )
            fprintf(fp, " %10d   %10d   %10d   %10d   %10lf   %10lf\n", process, numtasks, problemSize/numtasks, problemSize, local[process], total[process]);
        fprintf(fp, "--------------------------------------------------------------------------------\n");
        fclose(fp);
    }
    else{
        fprintf(stderr, "Can't open output file %s!\n", fileName);
        exit(1);        
    }
}

#endif
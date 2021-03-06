
//Main for "calculates-pi-value-parallel" C++ application
//Created by r4m0nl1m4 14/10/2020

#include <mpi.h>
#include "./report.h"

MPI_Status status;

int main(int argc, char** argv){

    /* Allocate serie environment variables */
    double problemSize, timeStart, timeEnd, executeTime, nLocal, x, step, sum, i;
    int taskid, numtasks;

    /* Getting the Problem Size */
    problemSize = atoi(argv[1]);

    /* Start parallel computing */
    MPI_Init(&argc, &argv);

    /* Blocks until all processes in the communicator have reached this routine. */
    MPI_Barrier(MPI_COMM_WORLD);

    /* Getting the Start Time */
    timeStart = MPI_Wtime();

    /* Getting the current processes (rank) */
    MPI_Comm_rank(MPI_COMM_WORLD, &taskid);

    /* Getting the number of processes (size) */
    MPI_Comm_size(MPI_COMM_WORLD, &numtasks);

    /* Getting the local problem size */
    nLocal = problemSize/numtasks;

    /* Discretization */
    step = 1/problemSize;
    sum = 0;
    for(i = taskid*nLocal+1; i<(taskid+1)*nLocal+1; i++){
        x = (i-0.5)*step;
        sum = sum + (1/(1+x*x));
    }
    /*
    printf("\nDiscretization:    ");
    printf("\n\t taskid = %d     ", taskid);
    printf("\n\t nLocal = %lf    ", nLocal);
    printf("\n\t interval %lf-%lf", taskid*nLocal+1, (taskid+1)*nLocal+1);
    printf("\n\t step = %lf\n    ", step);
    printf("\n\t x = %lf         ", x);
    printf("\n\t sum = %lf       ", sum);
    printf("\n");
    */

    /* Set parallel environment variables */
    bool isMaster;
    double local = sum;
    double total;
    double localVector[numtasks], totalVector[numtasks];

    isMaster = (taskid == 0);
    if(isMaster){
        int master = 0;
        localVector[master] = step*local;
        totalVector[master] = localVector[master];

        /* Sum */
        total = step*local;
        for( int worker=1; worker<numtasks; worker++ ){
            MPI_Recv(&local, 1, MPI_DOUBLE, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            total += step*local;

            localVector[worker] = 4*step*local;
            totalVector[worker] = 4*total;
        }
        total *= 4;

        /* Getting the End Time */
        timeEnd = MPI_Wtime();

        /* Getting the Time interval */
        executeTime = timeEnd-timeStart;

        saveCPUReportOnFile("result_report-parallel-cpu.txt", numtasks, (int) problemSize, localVector, totalVector, executeTime);
        saveResultReportOnFile("result_report-parallel-runtime.txt", executeTime);
    }
    else /* Send  local to process 0 */
        MPI_Send(&local, 1, MPI_DOUBLE, 0, i, MPI_COMM_WORLD);

    /* Blocks until all processes in the communicator have reached this routine. */
    MPI_Barrier(MPI_COMM_WORLD);

    /* End parallel computing */
   	MPI_Finalize();

  	return 0;
}

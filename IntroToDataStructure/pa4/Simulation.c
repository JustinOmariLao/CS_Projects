//-----------------------------------------------------------------------------
// SimulationStub.c
//-----------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include"Job.h"
#include"IntegerQueue.h"

//-----------------------------------------------------------------------------
// The following function may be of use in assembling the initial backup array.
// You may use it as is, alter it as you see fit, or delete it altogether.
//-----------------------------------------------------------------------------

// getjob()
// Reads two numbers from file in, and returns a new Job with those numbers
// as arrival and duration.
Job getJob(FILE* in){
    int a, d;
    fscanf(in, "%d %d\n", &a, &d);
    return newJob(a, d);
}

int openQueue(IntegerQueue* storageArray, int processor) {
    int isEmpty = 0;
    for (int i = 1; i <= processor; i++) {
        if (length(storageArray[i]) == 0) {
            isEmpty = 1;
            break;
        }
    }
    return isEmpty;
}

int getSmallestTime(IntegerQueue* storageArray, int jobIndex, Job* jobList) {
    int smallestTime = getFinish(jobList[peek(storageArray[1])]);
    for (int i = 1; i < jobIndex; i++) {
        if (smallestTime > getFinish(jobList[peek(storageArray[i])])) {
            smallestTime = getFinish(jobList[peek(storageArray[i])]);
        }
    }
    return smallestTime;
}

void queueParser(FILE* outFileTRC, char* currentQueue, Job* jobList) {
    char* queueString = strtok(currentQueue, " ");
    while (queueString != NULL) {
        printJob(outFileTRC, jobList[atoi(queueString)]);
        queueString = strtok(NULL, " ");
    }
    fprintf(outFileTRC, "\n");
}

void printQueues(FILE* outFileTRC, int time, IntegerQueue* storageArray, int processor, Job* jobList) {
    fprintf(outFileTRC, "time=%d\n", time);
    for(int i = 0; i <= processor; i++) {
        fprintf(outFileTRC, "%d: ", i );
        queueParser(outFileTRC, IntegerQueueToString(storageArray[i]), jobList);
    }
    fprintf(outFileTRC, "\n");
}

int getSmallestQueue(IntegerQueue* storageArray, int processor) {
    int smallestLength = length(storageArray[1]);
    int smallestIndex = 1;
    for (int i = 1; i <= processor; i++) {
        if (smallestLength > length(storageArray[i])) {
            smallestLength = length(storageArray[i]);
            smallestIndex = i;
        }
    }
    return smallestIndex;
}

int getTotalWait(Job* jobList, int m) {
    int totalWait = 0;
    for (int i = 0; i < m; i++ ) {
        totalWait = totalWait + getWaitTime(jobList[i]);
    }
    return totalWait;
}

int getMaxWait(Job* jobList, int m) {
    int maxWait = getWaitTime(jobList[0]);
    for (int i = 1; i < m; i++) {
        if (maxWait < getWaitTime(jobList[i])) {
            maxWait = getWaitTime(jobList[i]);
        }
    }
    return maxWait;
}

void removeFinishedJobs(IntegerQueue* storageArray, Job* jobList, int jobIndex, int time) {
    for (int i = 1; i < jobIndex; i++) {
        if (getFinish(jobList[peek(storageArray[i])]) < time) {
            enqueue(storageArray[0], dequeue(storageArray[i]));
        }
    }
}

void resetAll(IntegerQueue* storageArray, Job* jobList, int m, int processor) {
    int i;
    for (i = 0; i <= processor; i++) {
        dequeueAll(storageArray[i]);
    }
    for (i = 0; i <= m; i++) {
        resetFinishTime(jobList[i]);
    }
}

//-----------------------------------------------------------------------------
// The following stub for function main() contains a possible algorithm for
// this project.  Follow it if you like.  Note that there are no instructions
// included below for writing to either of the output files.  You must
// intersperse those commands as necessary.
//-----------------------------------------------------------------------------

// main()
int main(int argc, char* argv[]) {
    FILE *inputFile, *outFileRPT, *outFileTRC;
    IntegerQueue *storageArray;
    Job *jobList;
    char *fileSuffixRPT = ".rpt", *fileSuffixTRC = ".trc", *fileNameRPT, *fileNameTRC, *dummyFileName;
    int suffixSize = 4, m, i, processor, totalWait, maxWait, time, jobIndex;
    double avgWait;

    if(argc != 2) {
        return EXIT_FAILURE;
    }

    //Create two strings of argv[1] to use for creating names of output files
    dummyFileName = argv[1];
    fileNameRPT = calloc(strlen(argv[1]), sizeof(char));
    fileNameTRC = calloc(strlen(argv[1]), sizeof(char));
    for (i = 0; i <= strlen(dummyFileName); i++) {
        fileNameRPT[i] = dummyFileName[i];
    }
    for (i = 0; i <= strlen(dummyFileName); i++) {
        fileNameTRC[i] = dummyFileName[i];
    }

    fileNameRPT = strncat(fileNameRPT, fileSuffixRPT, suffixSize);
    fileNameTRC = strncat(fileNameTRC, fileSuffixTRC, suffixSize);
    inputFile = fopen(argv[1], "r");
    //Contains the results total wait, maximum wait, and average wait, for 𝑛 processors where 𝑛 ranges from 1 to 𝑚 − 1
    outFileRPT = fopen(fileNameRPT, "w");
    //Contain a detailed outFileTRC of the state of the simulation at those points in time when either an arrival or finish event occur.
    outFileTRC = fopen(fileNameTRC, "w");

    //Reading m number of jobs and placing the the job nodes into an array
    fscanf(inputFile, "%d", &m);
    jobList = calloc(m + 1, sizeof(Job));
    for(i = 0; i < m; i++) {
        jobList[i] =  getJob(inputFile);
    }
    storageArray = calloc(m - 1, sizeof(IntegerQueue));
    for (i = 0; i <= m - 1; i++) {
        storageArray[i] = newIntegerQueue();
    }

    //Report file start
    fprintf(outFileRPT, "Report file: %s\n", fileNameRPT);
    fprintf(outFileRPT, "%d Jobs:\n", m);
    for(i = 0; i <= m - 1; i++) {
        printJob(outFileRPT, jobList[i]);
    }
    fprintf(outFileRPT, "\n");
    fprintf(outFileRPT, "***********************************************************\n");

    //Trace file start
    fprintf(outFileTRC, "Trace file: %s\n", fileNameTRC);
    fprintf(outFileTRC, "%d Jobs:\n", m);
    for(i = 0; i <= m - 1; i++) {
        printJob(outFileTRC, jobList[i]);
    }
    fprintf(outFileTRC, "\n");

    // loop: run simulation with n processors for n=1 to n=m-1
    for (processor = 1; processor <= m - 1; processor++) {
        time = 0;
        jobIndex = 1;

        fprintf(outFileTRC, "*****************************\n");
        if(processor == 1) {
            fprintf(outFileTRC, "%d processor:\n", processor);
        }
        else {
            fprintf(outFileTRC, "%d processors:\n", processor);
        }
        fprintf(outFileTRC, "*****************************\n");

        for (i = 0; i <= m - 1; i++) {
            enqueue(storageArray[0], i);
        }

        printQueues(outFileTRC, time, storageArray, processor, jobList);

        for (i = 0; i <= m - 1; i++) {
            if (openQueue(storageArray, processor) == 1) {
                computeFinishTime(jobList[i], getArrival(jobList[peek(storageArray[0])]));
                enqueue(storageArray[getSmallestQueue(storageArray, processor)], dequeue(storageArray[0]));
                time = getSmallestTime(storageArray, jobIndex, jobList);
                if (time <= getFinish(jobList[i])) {
                    removeFinishedJobs(storageArray, jobList, jobIndex, getFinish(jobList[i]));
                }
                printQueues(outFileTRC, getFinish(jobList[i]), storageArray, processor, jobList);
                jobIndex++;
            }
            else {
                computeFinishTime(jobList[i], time);
                time = getFinish(jobList[i]);
                removeFinishedJobs(storageArray, jobList, jobIndex, time);
                enqueue(storageArray[getSmallestQueue(storageArray, processor)], dequeue(storageArray[0]));
                printQueues(outFileTRC, time, storageArray, processor, jobList);
            }
        }

        totalWait = getTotalWait(jobList, m);
        maxWait = getMaxWait(jobList, m);
        avgWait = (float) totalWait / (float) m;
        if (processor == 1) {
            fprintf(outFileRPT, "%d processor: totalWait=%d, maxWait=%d, avgWait=%.2f\n", processor, totalWait, maxWait, avgWait);
        }
        else {
            fprintf(outFileRPT, "%d processors: totalWait=%d, maxWait=%d, avgWait=%.2f\n", processor, totalWait, maxWait, avgWait);
        }
        resetAll(storageArray, jobList, m, processor);
    }

    fclose(inputFile);
    fclose(outFileRPT);
    fclose(outFileTRC);

    free(fileSuffixRPT);
    fileSuffixRPT = NULL;
    free(fileSuffixTRC);
    fileSuffixTRC = NULL;
    free(fileNameRPT);
    fileNameRPT = NULL;
    free(fileNameTRC);
    fileNameTRC = NULL;
    free(dummyFileName);
    dummyFileName = NULL;
    for (i = 0; i <= processor; i++) {
        free(storageArray[i]);
        storageArray[i] = NULL;
    }
    free(storageArray);
    storageArray = NULL;
    for (i = 0; i < m; i++) {
        free(jobList[i]);
        jobList[i] = NULL;
    }
    free(jobList);
    jobList = NULL;
    return EXIT_SUCCESS;
}

    // check command line arguments
    //
    // open files for reading and writing
    //
    // read in m jobs from input file and place them in backup array
    //
    // declare and initialize an array of m-1 processor Queues along with
    // any necessary storage queues
    //
    // loop: run simulation with n processors for n=1 to n=m-1 {
    //
    //    place Job indices from backup array into storage queue
    //
    //    loop: process all Jobs in this simulation {
    //
    //       determine the time of the next arrival or finish event and update time
    //
    //       if any Jobs finish now, then complete them in the order they appear in
    //       the queue array, i.e. lowest index first.
    //
    //       if any Jobs arrive now, then assign them to a processor queue
    //       of minimum length and with lowest index in the queue array.
    //
    //    } end loop
    //
    //    compute the total wait, maximum wait, and average wait for
    //    all Jobs, then reset finish times for next simulation
    //
    // } end loop
    //
    // free all heap memory, close input and output files
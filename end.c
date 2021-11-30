#include <errno.h>
#include <math.h> // log
#include <stdio.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include "helper.h"

int main(int argc, char *argv[]){
    int shmID;
    int infoID;
    int readCount;
    int* readptr;
    const char* filename = "save.txt";
    struct studentInfo* infoptr;
    FILE* save_file;

    for (int q = 0; q < argc; q++)
	{
		if (strcmp(argv[q], "-f") == 0){
            filename = argv[q + 1];
        }
	}
    sem_t *sem1 = sem_open("/order", 1);
    if(sem1 == SEM_FAILED){
        perror("order");
        exit(EXIT_FAILURE);
    }
    sem_t *sem2 = sem_open("/wrt", 1);
    if(sem2 == SEM_FAILED){
        perror("wrt");
        exit(EXIT_FAILURE);
    }
    sem_t *sem3 = sem_open("/mutex", 1);
    if(sem3 == SEM_FAILED){
        perror("mutex");
        exit(EXIT_FAILURE);
    }

    sem_t *sem4 = sem_open("/log", 1);
    if(sem4 == SEM_FAILED){
        perror("log");
        exit(EXIT_FAILURE);
    }

    printf("Please wait until all ongoing processes finished...\n");
    //remove shared memory of log data
    key_t key1;
    key1 = 9999; 
    shmID = shmget(key1, sizeof(struct logData) , 0666|IPC_CREAT);
    if (shmID < 0)
    {
        perror("shmget failure");
        exit(1);
    }
    struct logData *logptr = (struct logData *) shmat(shmID, NULL, 0);
    if(logptr < 0){
        printf("shmat() failed \n");
        exit(1);
    }
    key_t key2;
    key2 = 41777;
    infoID = shmget(key2, sizeof(struct studentInfo)*50, 0666|IPC_CREAT);
    if (infoID < 0) {
        perror("create: shmget failed");
        exit(1);
    }

    infoptr = (struct studentInfo*) shmat(infoID, 0, 0);
    if (infoptr <= (struct studentInfo*)(0)) {
        perror("create: shmat failed");
        exit(2);
    }
    

    key_t key3;
    key3 = 23;
    readCount = shmget(key3, sizeof(int), 0666|IPC_CREAT);
    if (readCount < 0) {
        perror("create: shmget failed for read_count");
        exit(1);
    }
    readptr = (int*)shmat(readCount, 0, 0);
    if (readptr <= (int*)(0)) {
        perror("create: shmat failed for read_count");
        exit(2);
    }


    save_file = fopen(filename, "w");
    if (save_file == NULL)
    {
        fprintf(stderr, "Couldn't open file\n");
        exit(1);
    }
    // printf("end test here!");
    sem_wait(sem1);
    sem_wait(sem3);

    *readptr += 1;
    if (*readptr == 1){
        sem_wait(sem2);
    }

    sem_post(sem1);
    sem_post(sem3);

    /*Beginning of critical section */
    printf("------------------------------Statistics:--------------------------\n");
    printf("number of readers processed: %d\n",logptr->reads);
    printf("average duration of readers: %.2lf\n",logptr->totalReadTime/logptr->reads);
    printf("number of writers processed: %d\n",logptr->writes);
    printf("average duration of writers: %.2lf\n",logptr->totalWriteTime/logptr->writes);
    printf("maximum delay recorded for starting the work of either a reader or a writer: %.2lf\n", logptr->max_delay);
    printf("sum number of records either accessed or modified: %d\n", logptr->ofRecsProcessed);
    int row = 0;
    while (strlen(infoptr[row].ID) > 1 && row <= 50) {
        fprintf(save_file, "%s,%s,%s,%.2lf\n",
            infoptr[row].ID, infoptr[row].name, infoptr[row].grades, infoptr[row].GPA);
        row += 1;
    }
    printf("---------------------------------------------------------------------\n");
    printf("Updated version of student record written in %s\n", filename);
    printf("You can check the logging data in the log file\n");
    printf("---------------------------------------------------------------------\n");
    /*End of critical section */
      
    sem_wait(sem3);
    *readptr -= 1;
    if (*readptr == 0){
        sem_post(sem2);
    }
    sem_post(sem3);

    
    //delete and detech shared memory
    shmdt(logptr); 
    shmdt(readptr); 
    shmdt(infoptr);
    shmctl(shmID,IPC_RMID,NULL);
    shmctl(infoID,IPC_RMID,NULL);
    shmctl(readCount,IPC_RMID,NULL);
    // Close the Semaphores
    sem_close(sem1);
    sem_unlink("/order");
    sem_close(sem2);
    sem_unlink("/wrt");
    sem_close(sem3);
    sem_unlink("/mutex");
    sem_close(sem4);
    sem_unlink("/log");
    printf("Program ended and shared memory cleaned...\n");
    

}

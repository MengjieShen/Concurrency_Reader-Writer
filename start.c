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
#include <semaphore.h>
#include "helper.h"
#define FACTOR 10e4 /* Used to generate random microseconds */

//sleeps for a random number of microseconds
void sleep_exp_time(void)
{
  unsigned int usecs = (-log(1.0 * rand() / RAND_MAX) / FACTOR * 10e6);
  usleep(usecs);
}

int main(){
    int shmID;
    int infoID;
    int readCount;
    int* readptr;
    sem_t *sem1;
    sem_t *sem2;
    sem_t *sem3;

    // initialize there semaphores

    sem1 = sem_open("/order", O_CREAT, 0666, 1);
    if(sem1 == SEM_FAILED){
        perror("order");
        exit(EXIT_FAILURE);
    }
    sem2 = sem_open("/wrt", O_CREAT, 0666, 1);
    if(sem2 == SEM_FAILED){
        perror("order");
        exit(EXIT_FAILURE);
    }
    sem3 = sem_open("/mutex", O_CREAT, 0666, 1);
    if(sem3 == SEM_FAILED){
        perror("order");
        exit(EXIT_FAILURE);
    }

    //initialize and allocate the shared memory segment for log Data
    key_t key1;
    key1 = 9999; 
    shmID = shmget(key1, sizeof(struct logData) , 0666|IPC_CREAT);

    //check for faiure (no segment found with that key)
    if (shmID < 0)
    {
        perror("shmget failure");
        exit(1);
    }

    //attach the memory segment
    struct logData *p = (struct logData *) shmat(shmID, NULL, 0);
    if((int) p < 0){
        printf("shmat() failed \n");
        exit(1);
    }

    key_t key2;
    key2 = 41777;
    //initialize and allocate the shared memory segment for student info data
    infoID = shmget(key2, sizeof(struct studentInfo)*50, 0666|IPC_CREAT);/* get shared memory to store data*/
    if (infoID < 0) {
        perror("create: shmget failed");
        exit(1);
    }

    key_t key3;
    key3 = 23;
    // Shared memory 3
    readCount = shmget(key3, sizeof(int), 0666|IPC_CREAT);
    if (readCount < 0) {
        perror("create: shmget failed for read_count");
        exit(1);
    }

    // attach read_count memory
    readptr = (int*)shmat(readCount, NULL, 0);
    if (readptr <= (int*)(0)) {
        perror("create: shmat failed for read_count");
        exit(2);
    }

    *readptr = 0;
    //close the semaphore
    // sem_close(sem1);
    // sem_close(sem2);
    // sem_close(sem3);
    printf("Shared memory initialized...\n");
    return 0;
}

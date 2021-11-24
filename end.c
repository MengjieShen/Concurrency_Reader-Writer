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

int main(){
    int shmID;
    int infoID;
    int readCount;
    int* readptr;
    sem_t *sem1;
    sem_t *sem2;
    sem_t *sem3;

    //remove shared memory of log data
    key_t key1;
    key1 = 9999; 
    shmID = shmget(key1, sizeof(struct logData) , 0666|IPC_CREAT);
    shmctl(shmID,IPC_RMID,NULL);

    //remove shared memory of student data
    key_t key2;
    key2 = 41777;
    infoID = shmget(key2, sizeof(struct studentInfo)*50, 0666|IPC_CREAT);
    shmctl(infoID,IPC_RMID,NULL);

    key_t key3;
    key3 = 23;
    readCount = shmget(key3, sizeof(int), 0666|IPC_CREAT);
    shmctl(readCount,IPC_RMID,NULL);

    
    // Close the Semaphores
    sem1 = sem_open("/order", 1);
    if(sem1 == SEM_FAILED){
        perror("order");
        exit(EXIT_FAILURE);
    }
    sem2 = sem_open("/wrt", 1);
    if(sem2 == SEM_FAILED){
        perror("order");
        exit(EXIT_FAILURE);
    }
    sem3 = sem_open("/mutex", 1);
    if(sem3 == SEM_FAILED){
        perror("order");
        exit(EXIT_FAILURE);
    }
    sem_close(sem1);
    sem_unlink("/order");
    sem_close(sem2);
    sem_unlink("/wrt");
    sem_close(sem3);
    sem_unlink("/mutex");
    printf("Shared memory cleaned...\n");
}

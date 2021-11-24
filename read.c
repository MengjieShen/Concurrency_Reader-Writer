#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "helper.h"
#define SHMSIZE 1024 //size of shared memory


int main() 
{ 
    int id;
    int shmid = 41777;
    key_t key;
    char *shm;

    sem_t *sem1 = sem_open("/order", O_CREAT, 0666, 1);
    sem_t *sem2 = sem_open("/wrt", O_CREAT, 0666, 1);
    sem_t *sem3 = sem_open("/mutex", O_CREAT, 0666, 1);

    sem_wait(sem1);
    sem_wait(sem3);
    sem_wait(sem1);

    id = shmget(shmid, sizeof(struct studentInfo)*50, IPC_CREAT | 0666);
    //Attach the segment to the data space
    shm = shmat(id , NULL , 0);

    //check for failure
    if (shm == (char *) -1)
    {
        perror("shmat failure");
        exit(1);
    }
  
    printf("Data read from memory: %s\n",shm); 
      
    //detach from shared memory  
    shmdt(shm); 
    
    // destroy the shared memory 
    shmctl(shmid,IPC_RMID,NULL); 
     
    return 0; 
} 
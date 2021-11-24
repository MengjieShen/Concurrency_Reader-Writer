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
    int shmid = 32;
    key_t key;
    char *shm;

    //We need the segment with key 9999 that was created by the writer process
    key = 9999; 

    //Attach the segment to the data space
    shm = shmat(shmid , NULL , 0);

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
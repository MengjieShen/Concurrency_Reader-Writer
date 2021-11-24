#include <sys/types.h>
#include <sys/ipc.h> 
#include <sys/shm.h> 
#include <stdio.h> 
#include <stdlib.h>
#include "helper.h"
#define SHMSIZE 1024 //size of shared memory
  
int main() 
{ 
    int shmid;
    key_t key;
    char *shm;

    key = 9999; //some arbitary key value to uniquely identify the shared memory segment
  
    //Create the shared memory segment
    shmid = shmget(key, SHMSIZE , IPC_CREAT | 0666);
  
    //check for faiure
    if (shmid < 0)
    {
        perror("shmget failure");
        exit(1);
    }

    //Attach the segment to the data space
    shm = shmat(shmid , NULL , 0);

    //check for failure
    if (shm == (char *) -1)
    {
        perror("shmat failure");
        exit(1);
    }
  
    printf("Write Data : "); 
    gets(shm);  //get input from user
  
    printf("Data written in memory: %s\n",shm); 
      
    //detach from shared memory  
    shmdt(shm); 
  
    return 0; 
} 

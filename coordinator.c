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

int shmID;

//sleeps for a random number of microseconds
void sleep_exp_time(void)
{
  unsigned int usecs = (-log(1.0 * rand() / RAND_MAX) / FACTOR * 10e6);
  usleep(usecs);
}

int main(int argc, char* argv[]){
    // initialize there semaphores
    sem_t *sem1;
    sem_t *sem2;
    sem_t *sem3;

    sem1 = sem_open("/order", O_CREAT, 0666, 1);
    sem2 = sem_open("/wrt", O_CREAT, 0666, 1);
    sem3 = sem_open("/mutex", O_CREAT, 0666, 1);

    //initialize and allocate the shared memory segment
    key_t key;
    key = 9999; 
    shmID = shmget(key, sizeof(struct Data) , 0666);

    //check for faiure (no segment found with that key)
    if (shmID < 0)
    {
        perror("shmget failure");
        exit(1);
    }

    //attach the memory segment
    struct Data *p = (struct Data *) shmat(shmID, NULL, 0);
    if((int) p < 0){
        printf("shmat() failed \n");
        exit(1);
    }

    shmdt(p);
    shmctl(shmID,IPC_RMID,NULL);

     // Close the Semaphores
    sem_close(sem1);
    sem_unlink("/order");
    sem_close(sem2);
    sem_unlink("/wrt");
    sem_close(sem3);
    sem_unlink("/mutex");

    return 0;
}

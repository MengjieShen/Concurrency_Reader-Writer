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
#include <sys/times.h> /* times() */ 
#include <string.h>
#include "helper.h"
  
int main(int argc, char *argv[]) 
{ 
    int lb;
	int ub;
	int shmid;
    int time;
    // int id;
    // char input[100];
    key_t key1, key2, key3;
    key1 = 9999;
    key2 = 41777;
    key3 = 23;
    int shmID;
    int infoID;
    int readCount;
    int* readptr;
    const char* fileName;
    struct studentInfo* infoptr;
    char* modi_grades[100];
	for (int q = 0; q < argc; q++)
	{
		if (strcmp(argv[q], "-f") == 0){
            fileName = argv[q + 1];
        }
		if (strcmp(argv[q], "-r") == 0){
            lb = atoi(argv[q + 1]);
			ub = atoi(argv[q + 2]);         
        }
		if (strcmp(argv[q], "-s") == 0){
            shmid = atoi(argv[q + 1]);
        }
		if (strcmp(argv[q], "-d") == 0){
            time = atoi(argv[q + 1]);
        }
	}

    double t1, t2, t3; 
    struct tms tb1, tb2, tb3; 
    double ticspersec;
    ticspersec = (double) sysconf(_SC_CLK_TCK); 
    t1 = (double) times(&tb1);
    printf("#########Program start... Please wait...##############\n");

    sem_t *sem1 = sem_open("/order", O_CREAT, 0666, 1);
    sem_t *sem2 = sem_open("/wrt", O_CREAT, 0666, 1);
    sem_t *sem3 = sem_open("/mutex", O_CREAT, 0666, 1);

    
    /* load the shared memory */
    shmID = shmget(key1, sizeof(struct logData) , 0666|IPC_CREAT);

    //check for faiure (no segment found with that key)
    if (shmID < 0)
    {
        perror("shmget failure");
        exit(1);
    }

    //attach the memory segment
    struct logData *logptr = (struct logData *) shmat(shmID, NULL, 0);
    if(logptr < 0){
        printf("shmat() failed \n");
        exit(1);
    }

    infoID = shmget(key2, sizeof(struct studentInfo)*50, 0666|IPC_CREAT);/* get shared memory to store data*/
    if (infoID < 0) {
        perror("create: shmget failed");
        exit(1);
    }

    infoptr = (struct studentInfo*) shmat(infoID, 0, 0);
    if (infoptr <= (struct studentInfo*)(0)) {
        perror("create: shmat failed");
        exit(2);
    }

    readCount = shmget(key3, sizeof(int), 0666|IPC_CREAT);
    if (readCount < 0) {
        perror("create: shmget failed for read_count");
        exit(1);
    }

    // attach read_count memory
    readptr = (int*)shmat(readCount, 0, 0);
    if (readptr <= (int*)(0)) {
        perror("create: shmat failed for read_count");
        exit(2);
    }


    sem_wait(sem1);
    sem_wait(sem2);
    sem_post(sem1);

    /* start of the critical section */
    t2 = (double) times(&tb2);
    int total_num;
    total_num = (rand() % (ub - lb + 1))+1;
    int record_list[total_num];
    for (int i = 0; i<total_num; i++){
        record_list[i] = (rand() %(ub - lb + 1)) + lb;
    }
    
    //remove duplicate 
    int i, j;
       for(i=0;i<total_num;i++){
           for(j=i+1;j<total_num;j++){
            if(record_list[i]==record_list[j]){
                record_list[j]=record_list[total_num-1];
                total_num--;
            }
        }
    }

    printf("You will change %d students record.\n", total_num);

    int list_count = 0;
    for (int i = 0; i < 50; i++){
        if (i == record_list[list_count]){
            // printf("here: %d", record_list[i]);
            printf("You will change student with ID %s with grades %s\n", infoptr[i].ID, infoptr[i].grades);
            printf("Please type in the updated grade and split with the blank space between each grade: \n Example: A A B+.\n"); 
            scanf("%[^\n]%*c", &modi_grades);
            strcpy(infoptr[i].grades, modi_grades);
            infoptr[i].GPA = GPA_calculator(modi_grades);
            printf("Data modified in memory: ID: %s Student Name: %s grades: %s GPA: %lf,\n",infoptr[i].ID, infoptr[i].name, infoptr[i].grades, infoptr[i].GPA);
            list_count ++;
            strcpy(modi_grades,"");
        }
        
        if(strncmp(infoptr[i].ID, "", 1) == 0){
            break;
        }
        
    }
    sleep(time);
    t3 = (double) times(&tb3);
    printf("Program ended! Initiation at %.2lf, Termination at %.2lf, waiting time: %.2lf, execution time: %.2lf.\n", t1/ticspersec, t3/ticspersec, (t2 - t1)/ticspersec, (t3-t2)/ticspersec);
    logptr->writes += 1;
    logptr->totalWriteTime += (t3-t2)/ticspersec;
    logptr ->ofRecsProcessed += total_num;
    if (logptr->max_delay < (t2 - t1)/ticspersec){
        logptr->max_delay = (t2 - t1)/ticspersec;
    }
    /* end of the critical section */
    sem_post(sem2);
    //detach from shared memory  

    shmdt(logptr); 
    shmdt(readptr); 
    shmdt(infoptr);
  
    return 0; 
} 

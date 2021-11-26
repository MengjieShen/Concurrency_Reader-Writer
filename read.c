#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/times.h> /* times() */ 
#include "helper.h"
#define SHMSIZE 1024 //size of shared memory


int main(int argc, char *argv[]) 
{ 
    srand(time(0));
    // check customized command
	int lb;
	int ub;
	// int shmid;
    int time;
    // const char* fileName;
    struct studentInfo* infoptr;
    char log[] = "log";
    int fd;
	for (int q = 0; q < argc; q++)
	{
		// if (strcmp(argv[q], "-f") == 0){
        //     fileName = argv[q + 1];
        // }
		if (strcmp(argv[q], "-r") == 0){
            lb = atoi(argv[q + 1]);
			ub = atoi(argv[q + 2]);         
        }
		// if (strcmp(argv[q], "-s") == 0){
        //     shmid = atoi(argv[q + 1]);
        // }
		if (strcmp(argv[q], "-d") == 0){
            time = atoi(argv[q + 1]);
        }
	}

    // int id;
    key_t key1, key2, key3;
    key1 = 9999;
    key2 = 41777;
    key3 = 23;
    int shmID;
    int infoID;
    int readCount;
    int* readptr;


    double t1, t2, t3; 
    struct tms tb1, tb2, tb3; 
    double ticspersec;
    ticspersec = (double) sysconf(_SC_CLK_TCK); 
    t1 = (double) times(&tb1);

    printf("----------------Program start... Please wait...----------------\n");
    sem_t *sem1 = sem_open("/order", O_CREAT, 0666, 1);
    sem_t *sem2 = sem_open("/wrt", O_CREAT, 0666, 1);
    sem_t *sem3 = sem_open("/mutex", O_CREAT, 0666, 1);
    sem_t *sem4 = sem_open("/log", O_CREAT, 0666, 1);

    /* open the log file */
    fd = open(log, O_WRONLY|O_CREAT|O_APPEND, 0666);
    if (fd < 0)
    {
        fprintf(stderr, "Couldn't open file\n");
        exit(1);
    }
    sem_wait(sem4);
    char writeIn1[100];
    sprintf(writeIn1, "Process %d entered at time %.lf Type: Read.\n ", getpid(), t1/ticspersec);
    write(fd, writeIn1, strlen(writeIn1));
    sem_post(sem4);
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
    sem_wait(sem3);

    *readptr += 1;
    if (*readptr == 1){
        sem_wait(sem2);
    }

    sem_post(sem1);
    sem_post(sem3);

    /*Beginning of critical section */
    t2 = (double) times(&tb2);
    sem_wait(sem4);
    char writeIn2[100];
    sprintf(writeIn2, "Process %d started to execute at %.lf Type: Read.\n ", getpid(), t2/ticspersec);
    write(fd, writeIn2, strlen(writeIn2));
    sem_post(sem4);
    int total_num;
    total_num = (rand() % (ub - lb + 1))+1;
    // printf("reader total num: %d", total_num);
    int record_list[total_num];
    for (int i = 0; i<total_num; i++){
        record_list[i] = (rand() %(ub - lb + 1)) + lb;
        // printf("test: %d \n",record_list[i]);
    }
    
    //remove duplicate 
    int i, j, temp;
       for(i=0;i<total_num;i++){
           for(j=i+1;j<total_num;j++){
            if(record_list[i]==record_list[j]){
                record_list[j]=record_list[total_num-1];
                total_num--;
            }
        }
    }
    //sort the list
    for (i = 0; i <=total_num - 1; i++)          
        for (j = total_num-2; j >=i; j--) 
            if (record_list[j]>record_list[j + 1])          
               {
               temp=record_list[j];
               record_list[j]=record_list[j+1];
               record_list[j+1]=temp;}

    int row_count = 0;
    int ind = 0;
    while (strlen(infoptr[row_count].ID) >= 1 && row_count <= 50 && ind < total_num) {
        if (row_count == record_list[ind]){
            printf("ID: %s Student Name: %s grades: %s GPA: %.2lf\n",
                    infoptr[row_count].ID, infoptr[row_count].name, infoptr[row_count].grades, infoptr[row_count].GPA);
            ind++;
        }
        row_count++;
    }


    // printf("total_num: %d\n", total_num);

    sleep(time);
    t3 = (double) times(&tb3);
    printf("------------------------Statistics--------------------------\n");
    printf("Initiation at %.2lf\nTermination at %.2lf\nwaiting time: %.2lf\nexecution time: %.2lf.\nProgram ended! \n", t1/ticspersec, t3/ticspersec, (t2 - t1)/ticspersec, (t3-t2)/ticspersec);
    sem_wait(sem4);
    char writeIn3[100];
    sprintf(writeIn3, "Process %d ended at %.lf Type: Read.\n ", getpid(), t3/ticspersec); 
    write(fd, writeIn3, strlen(writeIn3));
    sem_post(sem4);
    // fprintf(Log, "\nProgram type: reader Process ID: %d\n Execution starts at %.2lf\nterminated at %.2lf\nwaited for %.2f secaccessed %d records.\n", getpid(),t1/ticspersec, t3/ticspersec, (t2 - t1)/ticspersec, total_num);
    logptr->reads += 1;
    logptr->totalReadTime += (t3-t2)/ticspersec;
    logptr ->ofRecsProcessed += total_num;
    if (logptr->max_delay < (t2 - t1)/ticspersec){
        logptr->max_delay = (t2 - t1)/ticspersec;
    }
    /*End of critical section */
      
    sem_wait(sem3);
    *readptr -= 1;
    if (*readptr == 0){
        sem_post(sem2);
    }
    sem_post(sem3);
    
    //detach from shared memory  

    shmdt(logptr); 
    shmdt(readptr); 
    shmdt(infoptr);
    
    // destroy the shared memory 
    // shmctl(shmid,IPC_RMID,NULL); 
    close(fd);
    return 0; 
} 
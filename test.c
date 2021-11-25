#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
int main(){
    int total_num = 3;
    int record_list[3] = {1,2,2};
    int i, j,temp;
        for(i=0;i<total_num;i++){
            for(j=i+1;j<total_num;j++){
            if(record_list[i]==record_list[j]){
                record_list[j]=record_list[total_num-1];
                total_num--;
            }
        }
    }

    for (int i = 0; i< total_num; i++){
        printf("test:%d\n", record_list[i]);
    }
}

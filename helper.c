#include <stdio.h>
#include <string.h>
#include "helper.h"

float GPA(char* grade){

    if (strncmp(grade, "A-", 2) == 0){
        return 3.5;
    }
    else if((strncmp(grade, "A", 1) == 0)){
        return 4;
    }
    else if(strncmp(grade, "B+", 2) == 0){
        return 3;
    }
    else if((strncmp(grade, "B-", 2) == 0)){
        return 2;
    }
    else if((strncmp(grade, "B", 2) == 0)){
        return 2.5;
    }
    else if((strncmp(grade, "C+", 2) == 0)){
        return 1.5;
    }
    else if((strncmp(grade, "C-", 2) == 0)){
        return 0.5;
    }
    else if((strncmp(grade, "C", 1) == 0)){
        return 1;
    }
    else{
        return 0;
    }
}

float GPA_calculator(char* grade){
    char* temp[100];
    strcpy(temp, grade);
    float sum = 0;
    int count = 0;
    char * token = strtok(temp, " ");
    // loop through the string to extract all other tokens
    while( token != NULL ) {
    //   printf( " %s\n", token ); //printing each token
      sum +=  GPA(token);
    //   printf(" %.lf", sum);
      count ++;
      token = strtok(NULL, " ");
    }
    return sum/count;
}

#define DBSIZE 20
struct logData
{
    // short value;   /* Supposed data stored    */
    // long* activeProcess; /* Array of active processes */
    int reads;    /* Sum of read operations  */
    int writes;   /* Sum of write operations */
    int ofRecsProcessed; /* Record processed */
    float totalReadTime;
    float totalWriteTime;
    float max_delay;
};

struct studentInfo
{
    char ID[20];
    char name[20];
    char grades[50];
    float GPA;
};

float GPA(char* grade);
float GPA_calculator(char* grade);
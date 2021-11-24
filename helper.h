struct logData
{
    short value;   /* Supposed data stored    */
    long* activeProcess; /* Array of active processes */
    long reads;    /* Sum of read operations  */
    long writes;   /* Sum of write operations */
    long ofRecsProcessed; /* Record processed */
};

struct studentInfo
{
    char ID[20];
    char name[20];
    char grades[24];
    float GPA;
};
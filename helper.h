struct Data
{
  short value;   /* Supposed data stored    */
  long* activeProcess; /* Array of active processes */
  long reads;    /* Sum of read operations  */
  long writes;   /* Sum of write operations */
  long ofRecsProcessed; /* Record processed */
};
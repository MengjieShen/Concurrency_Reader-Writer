# Reader and Writer Problem without starvation
#### Mengjie Shen ms11141

## Design principle

I set up the three separate shared memory segments and four semaphores in ```start.c```. For three shared memory segments, one is for storing the statistics of the processes executed, one is for storing the student information, and the other one is for storing the read count variable. For four semaphores, there are two that we've discussed in class.```mutex``` ensures only one progress goes through and ```wrt``` ensures that the writer will be blocked if there is a reader going through. Since the mere two will favor reader and starve writer, I also added one called ```order``` in order to ensure the order of the reader and writer based on their arrival time, this can avoid the writer be executed in the finite time and avoid starvation. I also introduce one semaphore called ```log```, I use this semaphore to ensure the atomic execution of appending the log file and avoid the circumstances that several processes appending at the same time.

Since, I loaded up the student record information into the shared memory, therefore, every time, the reader or the writer need to access the record, they can directly get it from the shared memory instead of reading the student record file again. As a result, I made a small modifications regarding the interface design. Users need to specify the filename when calling the function start. For calling read function or write function, there's no need to input the filename since it's already in the shared memory. 

Since there are three separate shared memory segments, it's very unconvenient to input the shared memory id each time we call reader or writer. Therefore, I hard code the shared memory id in the ```start.c```, and the ids are shared among all the files. This can make the invocation of readers and writers more convenient.

I also implemented the logging mechanism in the form of an append-only file. It records the statistics of each process. It will append the log when program invokes, starts the critical section and program ends. I use one binary semaphore to ensure the atomic operation of appending to the log.

## How to compile and run my program
### First,compile the whole project use:
```make```

### Then, you need to initiate the program through command lines:
``` ./start -f filename```

a sample could be:

``` ./start -f student.txt```

-f indicates the name of the student record file that you want to load. The default file is ```student.txt```. Please follow the same structure of the file ```student.txt``` if you want to load other file.

### Next, you can use the function of reading records and writing records:
``` ./read -r lb ub -d time```

``` ./write -r lb ub -d time```

a sample could be:

``` ./read -r 1 3 -d 5```

``` ./write -r 1 3 -d 5```

-r  indicates the range (lower-bound and upper-bound) of random records to be looked-up from(Currently, it can support the number of records smaller or equal to 20!)
filename; the range always remains smaller or equal to the number of student records stored in filename
-d time provides the time period that the reader has to “work with the record(s)” 

Notice that, to update the record, please strictly follow the rule that you need to split each grade with the blank space, an example could be: A A B+
### Finally, you can call end function to purge shared memory segments and semaphores:
``` ./end -f filename```

You can call the function after you have done with reading and writing. It will end the program, purge the shared memory segments and semaphores, print out the program statistics and save the modified student record. It will save the updated student record in the filename you specified using interface.

The default file is ```save.txt```. You can also specify any other file that you want. If you input the same filename as the original student record, it will overwrite the original version.
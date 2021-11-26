# Reader and Writer Problem without starvation
#### Mengjie Shen ms11141

## Design principle

I implemented shared memory to store the semaphores, statistics. I also loaded up the student record information into the shared memory, therefore, every time, the reader or the writer need to access the record, they can directly get it from the shared memory instead of reading the student record file again. As a result, I made a small modifications regarding the interface design. Users need to specify the filename when calling the function start. For calling read function or write function, there's no need to input the filename since it's already in the shared memory. 

Semaphore-wise, I introduced three semaphores.



## How to compile and run my program
### First, you need to initiate the program through command lines:
``` ./start -f filename```

a sample could be:

``` ./start -f student.txt```

-f indicates the name of the student record file that you want to load. The default file is ```student.txt```. Please follow the same structure of the file ```student.txt``` if you want to load other file.

### Next, you can use the function of reading records and writing records.
``` ./read -r lb ub -d time```
``` ./write --r lb ub -d time```

a sample could be:

``` ./read -r 1 3 -d 5```

-r  indicates the range (lower-bound and upper-bound) of random records to be looked-up from
filename; the range always remains smaller or equal to the number of student records stored in filename
-d time provides the time period that the reader has to “work with the record(s)” 

### Finally, you can call end function to purge shared memory segments and semaphores
``` ./end -f filename```

You can call the function after you have done with reading and writing. It will end the program, purge the shared memory segments and semaphores, print out the program statistics and save the modified student record. It will save the updated student record in the filename you specified using interface.

The default file is ```save.txt```. You can also specify any other file that you want. If you input the same filename as the original student record, it will overwrite the original version.
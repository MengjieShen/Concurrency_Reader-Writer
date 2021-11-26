all: start end read write

start: start.c helper.h
	gcc -Wall -o start start.c -lpthread
end: end.c helper.h
	gcc -Wall -o end end.c -lpthread
read: read.c helper.h 
	gcc -Wall -o read read.c -lpthread
write: write.c helper.h 
	gcc -Wall -o write write.c helper.c -lpthread

reader: read 
	./read -r 0 2 -d 10

writer: write
	./write -r 0 2 -d 1
clean:
	rm -f start end read write log save.txt *~ core
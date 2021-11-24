all: start end read write

start: start.c helper.h
	gcc -Wall -o start start.c -lpthread
end: end.c helper.h
	gcc -Wall -o end end.c -lpthread
read: read.c helper.h 
	gcc -Wall -o read read.c -lpthread
write: write.c helper.h 
	gcc -Wall -o write write.c -lpthread
clean:
	rm -f start end read write *~ core
all: start end read write

start: start.c helper.h
	gcc -Wall -o start start.c -lpthread
end: end.c helper.h
	gcc -Wall -o end end.c -lpthread
read: read.c helper.h 
	gcc -Wall -o read read.c -lpthread
write: write.c helper.h 
	gcc -Wall -o write write.c -lpthread

reader: read 
	./read -f "student.txt" -r 0 2 -s 23 -d 1
clean:
	rm -f start end read write *~ core
cc=gcc -I../include

all: course.o

course.o: course.c
	$(cc) -c -o ../lib/$@ $^

clean:
	rm -f *.o
cc=gcc -I../include

all: student.o

student.o: student.c
	$(cc) -c -o ../lib/$@ $^

clean:
	rm -f *.o
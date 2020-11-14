cc=gcc -I../include

all: lib_db.a

lib_db.a: sqlite3.o db.o
	ar rcs ../lib/$@ $^

sqlite3.o: sqlite3.c
	$(cc) -c -o $@ $^ -ldl -lpthread -static

db.o: db.c
	$(cc) -c -o $@ $^

test: db.o sqlite3.o
	gcc $^ -o test.out -ldl -lpthread
	echo
	./test.out

clean:
	rm -rf *.out *.db *.o

.PHONY: clean
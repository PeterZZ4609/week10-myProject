all: libdb.a

libdb.a: sqlite3.o db.o
	ar rcs $@ $^

db.o: db.c
	gcc -c db.c -o $@ -I../include

sqlite3.o: sqlite3.c
	gcc -c $^ -o $@

test: db.o sqlite3.o
	gcc $^ -o test.out -ldl -lpthread
	echo
	./test.out

clean:
	rm -rf *.out *.db *.o

.PHONY: clean
all: libdb.a

db.o: db.c
	gcc -c db.c -o $@ -I../include

sqlite3.o: sqlite3.c
	gcc -c $^ -o $@

test: db.o sqlite3.o
	gcc $^ -o test.out -ldl -lpthread
	./test.out

clean:
	rm -rf *.o *.out *.db

.PHONY: clean
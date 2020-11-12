all: test

test: db.c
	gcc db.c sqlite3.c -o test -ldl -lpthread -I../include

clean:
	rm -rf test
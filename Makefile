cc=gcc -I "./include"

vpath %.o lib
vpath %.a lib

all: main

main: main.o
	mkdir lib || exit 0
	for dir in module_db module_student module_course; do $(MAKE) -C $$dir all || exit 1; done
	$(cc) -o main main.o ./lib/* -ldl -lpthread

main.o: main.c
	$(cc) -c -o $@ $^

clean:
	rm -f lib/*
	for dir in module_db module_student module_course; do $(MAKE) -C $$dir clean || exit 1; done

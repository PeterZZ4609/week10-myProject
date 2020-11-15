cc=gcc -I "./include"

all: main

main: main.o
	mkdir lib || exit 0
	for dir in module_db module_student module_course; do $(MAKE) -C $$dir all || exit 1; done
	$(cc) -o main main.o ./lib/* -ldl -lpthread

main.o: main.c
	$(cc) -c -o $@ $^

clean:
	rm -f lib/*.o *.o *.db
	for dir in module_db module_student module_course; do $(MAKE) -C $$dir clean || exit 1; done

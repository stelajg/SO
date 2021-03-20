build: so-cpp

so-cpp: hashmap.o main.o
	gcc -std=c89 -o so-cpp hashmap.o main.o

main.o: main.c main.h
	gcc -c main.c

hashmap.o: hashmap.c hashmap.h
	gcc -c hashmap.c

clean:
	rm -fr so-cpp
	rm -fr *.o
so-cpp: hashmap.o main.o
	gcc -std=c89 -o so-cpp hashmap.o main.o

main.o: main.c main.h
	gcc -c -o main.o main.c

hashmap.o: hashmap.c hashmap.h
	gcc -c -o hashmap.o hashmap.c

clean:
	rm -fr so-cpp
	rm -fr *.o

build: hashmap.o main.o
	gcc -g main.o hashmap.o -o so-cpp

main.o: main.c main.h
	gcc -c main.c

hashmap.o: hashmap.c hashmap.h
	gcc -c hashmap.c

clean:
	rm -fr so-cpp
	rm -fr *.o

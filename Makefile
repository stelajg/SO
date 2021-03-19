build: so-cpp

so-cpp: hashmap.c main.c
	gcc -std=c89 hashmap.c main.c -o so-cpp

clean:
	rm -fr so-cpp

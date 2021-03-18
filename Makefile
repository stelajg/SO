build: so-cpp

so-cpp: hashmap.c main.c
	gcc hashmap.c main.c -o so-cpp

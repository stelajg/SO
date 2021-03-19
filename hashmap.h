#ifndef HASHMAP_H
#define HASHMAP_H

#endif
#include <stdlib.h>

typedef struct HashMap{
    int hashcode;
    char* key;
    char* value;
    struct HashMap* next;
}HashMap;

void insertMap(HashMap** map, int hashcode, char* key, char* value);
void deleteMap(HashMap** head);
char* findInMap(HashMap** head, char* key);
int getKeys(HashMap* head, char** key_vector, int index);
int hashFunction(const char* key);
#ifndef HASHMAP_H
#define HASHMAP_H

#endif

#include <stdlib.h>

typedef struct HashMap {
    int hashcode;
    char *key;
    char *value;
    struct HashMap *next;
} HashMap;

char **splitInWord(char *text, int *numberOfWords);

char *helperInsertMap(HashMap **map, char *string_to_analyze);

void insertMap(HashMap **map, HashMap **head, int hashcode, char *key, char *value);

void deleteMap(HashMap **head);

void deleteNode(HashMap **head, char *key);

char *findInMap(HashMap **head, char *key);

char **getKeys(HashMap **map, int mapSize, int numberOfNodes);

int hashFunction(const char *key);
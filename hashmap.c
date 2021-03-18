//
// Created by stela on 3/14/21.
//
#include "hashmap.h"
#include <string.h>
#define _hashCode 26
#define $hashCode 27
#define uppercaseSubs 65
#define lowercaseSubs 97

void insertMap(HashMap** map, int hashcode, char* key, char* value){
    HashMap* new_node = (HashMap*)malloc(sizeof(HashMap));
    new_node->key = (char*)calloc((strlen(key) + 1),sizeof(char));
    strcpy(new_node->key, key);
    new_node->value = (char*)calloc((strlen(value) + 1),sizeof(char));
    strcpy(new_node->value, value);
    new_node->hashcode = hashcode;
    new_node->next = *map;
    *map = new_node;
}
void deleteMap(HashMap** head){
    HashMap* aux ;
    HashMap* next_node= (*head);
    if((*head) == NULL){
        return;
    }
    while(next_node != NULL) {
        aux = next_node;
        next_node = next_node->next;
        free(aux->value);
        free(aux->key);
        free(aux);
    }
}
int hashFunction(const char* key){
    int ascii_first_letter = (int)key[0];
    if(ascii_first_letter == 95)
        return _hashCode;
    if(ascii_first_letter == 36)
        return $hashCode;
    if(ascii_first_letter >= 65 && ascii_first_letter <= 90)
        return ascii_first_letter - uppercaseSubs;
    if(ascii_first_letter >= 97 && ascii_first_letter <= 122)
        return ascii_first_letter - lowercaseSubs;

    return -1;
}

int getKeys(HashMap* head, char** key_vector, int index){
    if(head == NULL)
        return 0;
    while(head != NULL){
        key_vector[index] = (char*)malloc((strlen(head->key)+1) * sizeof(char ));
        strcpy(key_vector[index], head->key);
        index++;
        head = head->next;
    }
    return index;
}
//void deleteNodeMap(HashMap** head, HashMap* node){
//    HashMap* aux = (HashMap*)malloc(sizeof(HashMap));
//    HashMap* map = (*head);
//    aux = NULL;
//    aux->next = map;
//    if(map == NULL){
//        return;
//    }
//    while(map->next != NULL){
//        if(strcmp(map->key, node->key) == 0){
//            if(aux == NULL){
//                HashMap* aux1 = map;
//                (*map) = (*map)->next;
//                aux1->next = NULL;
//
//            }
//        }
//    }
//}
#include "hashmap.h"
#include <string.h>

#define _hashCode 26
#define $hashCode 27
#define uppercaseSubs 65
#define lowercaseSubs 97

char **splitInWord(char *text, int *numberOfWords)
{
    int i = 0;
    char **result = NULL;
    char *pch = NULL;

    while (text[i] != '\0') {
        if (text[i] == ' ')
            (*numberOfWords)++;
        i++;
    }
    result = (char **) calloc((*numberOfWords), sizeof(char *));
    pch = strtok(text, " ");
    i = 0;
    while (pch != NULL) {
        result[i] = (char *) calloc(strlen(pch) + 1, sizeof(char));
        strcpy(result[i], pch);
        pch = strtok(NULL, " ");
        i++;
    }
    return result;
}

char *helperInsertMap(HashMap **map, char *string_to_analyze) {
    char *result = NULL;
    int number_words = 1;
    char **array_words = NULL;
    int i;
    size_t resultSize = 0;

    array_words = splitInWord(string_to_analyze, &number_words);
    for (i = 0; i < number_words; i++) {
        int hashAux = hashFunction(array_words[i]);

        if (hashAux != -1) {
            char *aux = findInMap(&map[hashAux], array_words[i]);

            if (aux != NULL) {
                free(array_words[i]);
                array_words[i] = (char *) calloc(strlen(aux) + 1, sizeof(char));
                strcpy(array_words[i], aux);
            }
        }
        resultSize += strlen(array_words[i]) + 1;
    }
    result = (char *) calloc(resultSize, sizeof(char));
    for (i = 0; i < number_words; i++) {
        strcat(result, array_words[i]);
        free(array_words[i]);
        if (i < number_words - 1)
            strcat(result, " ");
    }
    free(array_words);
    return result;
}

void insertMap(HashMap **map, HashMap **head, int hashcode, char *key, char *value) {
    HashMap *new_node = (HashMap *) malloc(sizeof(HashMap));
    new_node->key = (char *) calloc((strlen(key) + 1), sizeof(char));
    strcpy(new_node->key, key);
    if (strcmp(value, "") == 0) {
        new_node->value = (char *) calloc((strlen(value) + 1), sizeof(char));
        strcpy(new_node->value, value);
    } else {
        char *aux = helperInsertMap(map, value);
        new_node->value = (char *) calloc((strlen(aux) + 1), sizeof(char));
        strcpy(new_node->value, aux);
        free(aux);
    }
    new_node->hashcode = hashcode;
    new_node->next = *head;
    *head = new_node;
}

void deleteMap(HashMap **head) {
    HashMap *aux;
    HashMap *next_node = (*head);
    if ((*head) == NULL)
        return;
    while (next_node != NULL) {
        aux = next_node;
        next_node = next_node->next;
        free(aux->value);
        free(aux->key);
        free(aux);
    }
}

int hashFunction(const char *key) {
    int ascii_first_letter = (int) key[0];
    if (ascii_first_letter == 95)
        return _hashCode;
    if (ascii_first_letter == 36)
        return $hashCode;
    if (ascii_first_letter >= 65 && ascii_first_letter <= 90)
        return ascii_first_letter - uppercaseSubs;
    if (ascii_first_letter >= 97 && ascii_first_letter <= 122)
        return ascii_first_letter - lowercaseSubs;

    return -1;
}

char **getKeys(HashMap **map, int mapSize, int numberOfNodes) {
    char **key_vector = (char **) calloc(numberOfNodes, sizeof(char *));
    int i = 0;
    int index = 0;
    HashMap *head = NULL;
    for (i = 0; i < mapSize; i++) {
        if (map[i] != NULL) {
            head = map[i];
            while (head != NULL) {
                key_vector[index] = (char *) calloc((strlen(head->key) + 1), sizeof(char));
                strcpy(key_vector[index], head->key);
                index++;
                head = head->next;
            }
        }
    }
    return key_vector;
}

char *findInMap(HashMap **head, char *key) {
    HashMap *aux = (*head);
    while (aux != NULL) {
        if (strcmp(key, aux->key) == 0)
            return aux->value;
        aux = aux->next;
    }
    return NULL;
}

void deleteNode(HashMap **head, char *key) {
    HashMap *temp = *head;
    HashMap *prev;

    if (temp != NULL && (strcmp(temp->key, key) == 0)) {
        (*head) = temp->next;
        free(temp->key);
        free(temp->value);
        free(temp);
        return;
    }

    while (temp != NULL && (strcmp(temp->key, key) != 0)) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL)
        return;

    prev->next = temp->next;

    free(temp->key);
    free(temp->value);
    free(temp);
}

#include <stdio.h>
#include <string.h>
#include "hashmap.h"
#define MapSize 28
void addArgsMap(HashMap** map, char* symbol_mapping, int pos, int* map_size){
    char *equal_symbol = strchr(symbol_mapping, '=');
    if (equal_symbol == NULL) {
        int hashcode = hashFunction(symbol_mapping + pos);
        if(hashcode == -1){
            printf("%s","Incorrect define symbol");
            return;
        }
        insertMap(&map[hashcode], hashcode, symbol_mapping + pos, "");
        (*map_size)++;
    } else {
        u_long key_size = strlen(symbol_mapping) - strlen(equal_symbol) - pos + 1;
        char *aux = (char*)calloc(key_size,sizeof(char ));
        strncpy(aux, symbol_mapping + pos, key_size );
        aux[key_size - 1] = '\0';
        int hashcode = hashFunction(aux);
        if(hashcode == -1){
            printf("%s","Incorrect define symbol");
            return;
        }
        insertMap(&map[hashcode], hashcode, aux, equal_symbol + 1);
        free(aux);
        (*map_size)++;
    }
}

void parseInputArgs(char* argv[], int argc, HashMap** map, char** fd_src_address,
                    char **fd_drc_address, char** dir_address, int* map_size){
    int wasD = 0;
    int wasI = 0;
    int wasS = 0;
    int wasO = 0;
    for(int i = 1; i < argc; i++) {
        if(strstr(argv[i], ".c") != NULL){
            (*fd_src_address) = (char*)calloc((strlen(argv[i])+1), sizeof(char ));
            strcpy((*fd_src_address), argv[i]);
            wasS = 1;
        }
        if(memcmp(argv[i], "-o", 2) == 0){
            size_t size_argv = strlen(argv[i]);
            if(size_argv == 2) {
                i++;
                (*fd_drc_address) = (char *)calloc((strlen(argv[i])+1), sizeof(char));
                strcpy((*fd_drc_address), argv[i]);
            }else{
                size_t address = size_argv - 1;
                (*fd_drc_address) = (char *)calloc(address, sizeof(char));
                strncpy((*fd_drc_address), argv[i] + 2, address);
                (*fd_drc_address)[address - 1] = '\0';
            }
            wasO = 1;
        }
        if(wasO == 1)
            break;
        if(memcmp(argv[i], "-D", 2) == 0) {
            wasD = 1;
            if (strlen(argv[i]) > 2) {
                addArgsMap(map, argv[i], 2, map_size);
            }
            i++;
        }
        if(memcmp(argv[i], "-I", 2) == 0){
            wasI = 1;
            if(strlen(argv[i]) == 2){
                i++;
                (*dir_address) = (char *) calloc((strlen(argv[i]) + 1), sizeof(char));
                strcpy((*dir_address),argv[i]);
            }else{
                size_t address = strlen(argv[i]) - 2 + 1;
                (*dir_address) = (char *) calloc(address, sizeof(char));
                strcpy((*dir_address), argv[i] + 2);
            }
        }
        if(wasD == 1 && wasI == 0 && wasS == 0){
            addArgsMap(map, argv[i], 0, map_size);
        }
    }
    if(wasS == 0){
        (*fd_src_address) = (char*)malloc((strlen("STDIN") + 1) * sizeof(char ));
        strcpy((*fd_src_address), "STDIN");
    }
}
void getKeysMap(HashMap** map, int number_of_elements, char**keys){
    int j = 0;
    for(int i = 0; i < MapSize; i++){
        j = getKeys(map[i], keys, j);
    }
}

int main(int argc, char* argv[]) {
    FILE* fd_src;
    FILE* fd_dsc;
    char *fd_src_address = NULL;
    char *fd_drc_address = NULL;
    char *dir_address = NULL;
    HashMap** map = (HashMap**)calloc(MapSize, sizeof(HashMap*));
    int map_size = 0;
    if(argc == 1){
        printf("Not enough arguments");
        return 0;
    }
    else{
        parseInputArgs(argv, argc, map, &fd_src_address, &fd_drc_address, &dir_address, &map_size);
    }
    char** keys = (char**)calloc(map_size, sizeof(char*));
    getKeysMap(map,map_size, keys);
    for(int i = 0; i < map_size; i++){
        printf("%s\n", keys[i]);
    }
    printf("%s\n", fd_src_address);
    printf("_____________________________________________________________");
    printf("%s\n", fd_drc_address);
    printf("_____________________________________________________________");
    printf("%s\n", dir_address);
    for(int i = 0; i < map_size; i++){
        free(keys[i]);
    }
    free(keys);
    free(fd_src_address);
    free(fd_drc_address);
    free(dir_address);
    for(int i = 0; i < MapSize; i++){
        deleteMap(&map[i]);
    }
    free(map);
    return 0;
}

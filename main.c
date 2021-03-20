#include <stdio.h>
#include <string.h>
#include "hashmap.h"
#define MapSize 28
#define TEXTBUFFERSIZE 100

void addArgsMap(HashMap** map, char* symbol_mapping, int pos, int* map_size){
    char *equal_symbol = strchr(symbol_mapping, '=');
    if (equal_symbol == NULL) {
        int hashcode = hashFunction(symbol_mapping + pos);
        if(hashcode == -1){
            printf("%s","Incorrect define symbol");
            return;
        }
        insertMap(map, &map[hashcode], hashcode, symbol_mapping + pos, "");
        (*map_size)++;
    } else {
        size_t key_size = strlen(symbol_mapping) - strlen(equal_symbol) - pos + 1;
        char *aux = (char*)calloc(key_size,sizeof(char ));
        strncpy(aux, symbol_mapping + pos, key_size );
        aux[key_size - 1] = '\0';
        int hashcode = hashFunction(aux);
        if(hashcode == -1){
            printf("%s","Incorrect define symbol");
            return;
        }
        insertMap(map, &map[hashcode], hashcode, aux, equal_symbol + 1);
        free(aux);
        (*map_size)++;
    }
}

void parseInputArgs(char** argv, int argc, HashMap** map, char** fd_src_address,
                    char **fd_drc_address, char** dir_address, int* map_size){
    int wasS = 0;
    int wasO = 0;
    int i = 1;
    while(i < argc ) {
        /*fprintf(stderr,"%s","argumente:");
        fprintf(stdout,"%s",argv[i]);
         */
        if(memcmp(argv[i], "-D", 2) == 0) {
            if (strlen(argv[i]) > 2) {
                addArgsMap(map, argv[i], 2, map_size);
                i++;
            }else{
                i++;
                addArgsMap(map, argv[i], 0, map_size);
                i++;
            }
        }else if(memcmp(argv[i], "-I", 2) == 0){
            if(strlen(argv[i]) == 2){
                i++;
                (*dir_address) = (char *) calloc((strlen(argv[i]) + 1), sizeof(char));
                strcpy((*dir_address),argv[i]);
                i++;
            }else{
                size_t address = strlen(argv[i]) - 2 + 1;
                (*dir_address) = (char *) calloc(address, sizeof(char));
                strcpy((*dir_address), argv[i] + 2);
                i++;
            }
        }else if(memcmp(argv[i], "-o", 2) == 0){
            wasO = 1;
            size_t size_argv = strlen(argv[i]);
            if(size_argv == 2) {
                i++;
                (*fd_drc_address) = (char *) calloc((strlen(argv[i]) + 1), sizeof(char));
                strcpy((*fd_drc_address), argv[i]);
                i++;
            }else{
                size_t address = size_argv - 1;
                (*fd_drc_address) = (char *)calloc(address, sizeof(char));
                strncpy((*fd_drc_address), argv[i] + 2, address);
                (*fd_drc_address)[address - 1] = '\0';
                i++;
            }

        } else if(wasS == 0){
            (*fd_src_address) = (char*)calloc((strlen(argv[i])+1), sizeof(char));
            strcpy((*fd_src_address), argv[i]);
            wasS = 1;
            i++;
        } else{
            if(wasO == 1){
                exit(1);
            }
            (*fd_drc_address) = (char *) calloc((strlen(argv[i]) + 1), sizeof(char));
            strcpy((*fd_drc_address), argv[i]);
            wasO = 1;
            i++;
        }
    }
    if(wasS == 0){
        (*fd_src_address) = (char*)malloc((strlen("STDIN") + 1) * sizeof(char ));
        strcpy((*fd_src_address), "STDIN");
    }
    if(wasO == 0){
        (*fd_drc_address) = (char*)malloc((strlen("STDOUT") + 1) * sizeof(char ));
        strcpy((*fd_drc_address), "STDOUT");
    }
}

void helperAnalyzeFILEInput(FILE* fd_src, HashMap** map, char* text, int* mapSize){
    /*pch = strtok (text," \n");
    pch = strtok (NULL, " \n");
    int hashcode = hashFunction(pch);
     */
    /*aici trebuie de revizuit, poate sa alegi doar alphanumeri+simboluri, ai prea multe spatii,exercitiu 12*/
    if(strchr(text, '\\') != NULL){
        char**aux_text = (char**)calloc(2,sizeof(char*));
        aux_text[0] = (char*)calloc(strlen(text) - 1,sizeof(char));
        strncpy(aux_text[0], text, strlen(text) - 1);
        aux_text[0][strlen(text) - 2] = '\0';
        int i = 1;
        size_t size = 0;
        int size_aux_text = 2;
        size_t new_string_size = strlen(aux_text[0]) + 1;
        while(strchr(text, '\\') != NULL){
            if(i == size_aux_text){
                size_aux_text += 2;
                aux_text = (char **)realloc(aux_text, sizeof(char *)*2);
            }
            getline(&text,&size,fd_src);
            aux_text[i] = (char*)calloc(strlen(text) - 1,sizeof(char));
            strncpy(aux_text[i], text, strlen(text) - 1);
            aux_text[i][strlen(text) - 2] = '\0';
            new_string_size += strlen(aux_text[i]);
            i++;
        }
        free(text);
        text = (char*)calloc(new_string_size, sizeof(char));
        int j;
        for(j = 0; j < i; j++){
            strcat(text, aux_text[j]);
            /*free(aux_text[j]);*/
        }
        free(aux_text);
    }
    char* aux = strchr(text,' ');
    char* aux1 = strchr(aux + 1,' ');
    char* key = (char*)calloc((strlen(aux)- strlen(aux1) + 1), sizeof(char ));
    memcpy(key,aux + 1, (strlen(aux)- strlen(aux1)));
    key[strlen(key) - 1] = '\0';
    char*value = (char *)calloc( strlen(aux1) + 1, sizeof(char ));
    memcpy(value,aux1 + 1, strlen(aux1)-1);
    value[strlen(value)-1] = '\0';
    int hashcode = hashFunction(key);
    insertMap(map, &map[hashcode], hashcode, key, value);
    (*mapSize)++;
    free(key);
    free(value);
}

void writeFILE(FILE* dst, HashMap** map, char**keys, char* text, int map_size){
    int i;
    int wasFound = 0;
    int isInAQuote = 0;
    for(i = 0; i < map_size; i++){
        char *find = strstr(text, keys[i]);
        if( find != NULL){
            char *findQuote = strchr(find, '"');
            if(findQuote != NULL){
                char *findSecondQuote  = strchr(findQuote + 1, '"');
                if(findSecondQuote == NULL){
                    isInAQuote = 1;
                }
            }
            if(isInAQuote == 0) {
                size_t aux_size = strlen(text) - strlen(find);
                char *aux = (char *) calloc(aux_size, sizeof(char));
                strncpy(aux, text, aux_size);
                aux[aux_size - 1] = '\0';
                fprintf(dst, "%s ", aux);
                fprintf(dst, "%s", findInMap(&map[hashFunction(keys[i])], keys[i]));
                fprintf(dst, "%s", find + strlen(keys[i]));
                free(aux);
                wasFound = 1;
            }
        }
    }
    if(wasFound == 0){
        fprintf(dst, "%s",text);
    }
}

void deleteKeys(HashMap**map, char* text){
    char * pch;
    pch = strtok (text," \n");
    pch = strtok (NULL, " \n");
    deleteNode(&map[hashFunction(pch)], pch);
}

long int findIfExistsInMap(HashMap** map, char** text, int isForIfdef){
    char * pch;
    pch = strtok ((*text)," \n");
    pch = strtok (NULL, " \n");
    char* cEnd;
    long int if_var;
    int hash = hashFunction(pch);
    char* aux = NULL;
    if(hash != -1) {
        aux = findInMap(&map[hashFunction(pch)], pch);
    }
    if(aux != NULL && hash != -1){
        if(isForIfdef == 1){
            return 1;
        }
        if_var = strtol(aux, &cEnd,10);
    }else{
        if(isForIfdef == 1){
            return -1;
        }
        if_var = strtol(pch, &cEnd,10);
    }
    return if_var;
}

void helperAnalyzerIF_ENDIF(FILE** fd_src, FILE* fd_dst, char** text, HashMap** map, char** keys, int map_size){
    long int if_var = findIfExistsInMap(map,text,0);
    size_t size = 0;
    free((*text));
    getline(text,&size,(*fd_src));
    int wasElse = 0;
    while(strstr((*text), "#endif") == NULL){
        if(if_var == 0 && strstr((*text), "#elif") != NULL){
            if_var = findIfExistsInMap(map,text,0);
        }else if(strstr((*text), "#else") != NULL){
            wasElse = 1;
        }else if(wasElse == 1 && if_var == 0){
            writeFILE(fd_dst,map,keys,(*text), map_size);
        }else if(if_var != 0 && wasElse == 0){
            writeFILE(fd_dst,map,keys,(*text), map_size);
        }
        getline(text,&size,(*fd_src));
    }
    getline(text,&size,(*fd_src));
}

void helperAnalyzerIFDEF_ENDIF(FILE** fd_src, FILE* fd_dst, char** text, HashMap**map, char** keys,
                                                                                            int map_size, int forIfndef){
    long int if_var = findIfExistsInMap(map,text,1);
    if(forIfndef == 1){
        if(if_var == -1){
            if_var = 1;
        }else{
            if_var = -1;
        }
    }
    size_t size = 0;
    free((*text));
    getline(text,&size,(*fd_src));
    int wasElse = 0;
    while(strstr((*text), "#endif") == NULL){
        if(if_var == -1 && strstr((*text), "#elif") != NULL){
            if_var = findIfExistsInMap(map,text,1);
        }else if(strstr((*text), "#else") != NULL){
            wasElse = 1;
        }else if(wasElse == 1 && if_var == -1){
            writeFILE(fd_dst,map,keys,(*text), map_size);
        }else if(if_var == 1 && wasElse == 0){
            writeFILE(fd_dst,map,keys,(*text), map_size);
        }
        getline(text,&size,(*fd_src));
    }
    getline(text,&size,(*fd_src));
}

void analyzerFileInput(FILE* fd_src, FILE* fd_dst,HashMap**map, int map_size){
    char** keys = NULL;
    int readKeys = 0;
    int readUndef = 0;
    int i;
    char* text = NULL;
    size_t size = 0;
    size_t read = -1;

    while((read = getline(&text,&size,fd_src)) != - 1){
        if(feof(fd_src))
            break;
        if(readUndef == 1){
            readUndef = 0;
        }
        if(feof(fd_src))
            break;
        if(strstr(text, "#ifndef") != NULL){
            helperAnalyzerIFDEF_ENDIF(&fd_src,fd_dst,&text,map,keys,map_size,1);
        }else if(strstr(text, "#ifdef") != NULL){
            helperAnalyzerIFDEF_ENDIF(&fd_src,fd_dst,&text,map,keys,map_size,0);
        }
        else if(strstr(text, "#define") != NULL){
            helperAnalyzeFILEInput(fd_src, map, text, &map_size);
        }else if(strstr(text, "#undef") != NULL){
            deleteKeys(map,text);
            for(i = 0; i < map_size; i++){
                free(keys[i]);
            }
            free(keys);
            keys = getKeys(map, MapSize, map_size);
            map_size--;
            readUndef = 1;
        }else if(strstr(text, "#if") != NULL){
            helperAnalyzerIF_ENDIF(&fd_src,fd_dst,&text,map,keys,map_size);
        }else if(readKeys == 0){
            readKeys = 1;
            keys = getKeys(map, MapSize,map_size);
        }
        if(readKeys == 1 && readUndef == 0)
            writeFILE(fd_dst,map,keys,text, map_size);
    }
    for(i = 0; i < map_size; i++){
        free(keys[i]);
    }
    if(text != NULL)
        free(text);
    free(keys);
}
int main(int argc, char** argv) {
    FILE* fd_src;
    FILE* fd_dst;
    char *fd_src_address = NULL;
    char *fd_drc_address = NULL;
    char *dir_address = NULL;
    char* text = NULL;
    size_t size = 0;
    size_t read;
    int i;
    HashMap** map = (HashMap**)calloc(MapSize, sizeof(HashMap*));
    int map_size = 0;
    char** keys = NULL;
    parseInputArgs(argv, argc, map, &fd_src_address, &fd_drc_address, &dir_address, &map_size);
    if(strcmp(fd_drc_address, "STDOUT") == 0){
        fd_dst = stdout;
    }else {
        fd_dst = fopen(fd_drc_address, "w+");
        if (fd_dst == NULL) {
            exit(-1);
        }
    }
    int isInStdin = 0;
    if(strcmp(fd_src_address, "STDIN") == 0){
        fd_src = stdin;
    }else {
        fd_src = fopen(fd_src_address, "r");
        if (fd_src == NULL) {
            exit(-1);
        }
    }
    char test[60];

    if( fgets (test, 60, fd_src) != NULL ) {
        rewind(fd_src);
        analyzerFileInput(fd_src,fd_dst,map,map_size);
    }

    if(fd_src != NULL)
        fclose(fd_src);
    if(fd_dst != NULL)
        fclose(fd_dst);

    if(fd_src_address != NULL)
        free(fd_src_address);
    if(fd_drc_address != NULL)
        free(fd_drc_address);
    if(fd_drc_address)
        free(dir_address);

    for(i = 0; i < MapSize; i++){
        deleteMap(&map[i]);
    }
    free(map);
    return 0;
}

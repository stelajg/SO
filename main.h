#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
//#include <dirent.h>
#include "hashmap.h"
#define MapSize 28
#define TEXTBUFFERSIZE 100

void addArgsMap(HashMap** map, char* symbol_mapping, int pos, int* map_size);
void parseInputArgs(char** argv, int argc, HashMap** map, char** fd_src_address,
                    char **fd_drc_address, char** dir_address, int* map_size);
void helperAnalyzeFILEInput(FILE* fd_src, HashMap** map, char* text, int* mapSize);
void writeFILE(FILE* dst, HashMap** map, char**keys, char* text, int map_size);
void deleteKeys(HashMap**map, char* text);
long int findIfExistsInMap(HashMap** map, char** text, int isForIfdef);
void helperAnalyzerIF_ENDIF(FILE** fd_src, FILE* fd_dst, char** text, HashMap** map, char** keys, int map_size);
void helperAnalyzerIFDEF_ENDIF(FILE** fd_src, FILE* fd_dst, char** text, HashMap**map, char** keys,
                               int map_size, int forIfndef);
void analyzerFileInput(FILE* fd_src, FILE* fd_dst,char* dir_address,HashMap**map, int map_size);
void helperAnalyzerInclude(FILE* fd_dst, char* text, char* dir_address,HashMap**map, int map_size);
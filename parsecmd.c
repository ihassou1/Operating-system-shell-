/*
 * Swarthmore College, CS 31
 * Copyright (c) 2021 Swarthmore College Computer Science Department,
 * Swarthmore PA
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "parsecmd.h"

char **parse_cmd_dynamic(const char *cmdline, int *bg) {


    char **result = NULL;
    *bg=0;
    int current = 1;
    int argCounter = 0;

    //0; space, 1; letter
    if(!isspace(cmdline[0])) { argCounter++; }
      while (cmdline[current] != '\0') {
            if( isspace(cmdline[current-1]))  {
                if( !isspace(cmdline[current])) {
                    if (cmdline[current]=='&'){
                      *bg=1;
                      break;
                    }
                    argCounter++;
            }
        }
        current++;
    }
    result = malloc(sizeof(char*)*(argCounter+1));
    current = 1;

    int resultIndex = 0;
    int charCounter = 0;
    int * sizeOfArguments=malloc(sizeof(int)* argCounter);
    if(!isspace(cmdline[0])) {
            charCounter++;
        while ( !isspace(cmdline[current])) {
            charCounter++;
            current++;
        }
    result[resultIndex] = malloc(sizeof(char)*(charCounter+1));
    sizeOfArguments[resultIndex]=charCounter;
    resultIndex++;
    charCounter = 0;
    }


    while (cmdline[current] != '\0') {
        if(isspace(cmdline[current-1]) && !isspace(cmdline[current])) {
                if (cmdline[current]=='&'){
                  break;
                }
                while ( !isspace(cmdline[current])) {
                    charCounter++;
                    current++;
                }
            result[resultIndex] = malloc(sizeof(char)*(charCounter+1));
            sizeOfArguments[resultIndex]=charCounter;
            resultIndex++;
            charCounter = 0;
            }
        current++;
    }
    result[resultIndex]=NULL;


    current = 1;

    resultIndex = 0;
    int currWordSize;
    if(!isspace(cmdline[0])) {
        currWordSize=sizeOfArguments[resultIndex];
        result[resultIndex][0]=cmdline[0];
        for (int i=1;i<currWordSize;i++){
          result[resultIndex][i]=cmdline[current];
          current++;
        }
        result[resultIndex][currWordSize]='\0';
        resultIndex++;
    }


    while (cmdline[current] != '\0') {
        if(isspace(cmdline[current-1]) && !isspace(cmdline[current])) {
          if (cmdline[current]=='&'){
            break;
          }
          currWordSize=sizeOfArguments[resultIndex];
                for (int i=0;i<currWordSize;i++){
                  result[resultIndex][i]=cmdline[current];
                  current++;
                }
                result[resultIndex][currWordSize]='\0';
                resultIndex++;
            }
        current++;
    }
    free(sizeOfArguments);


    return result;
}

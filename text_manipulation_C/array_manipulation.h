#ifndef _ARRAY_MANIPULATION_H
#define _ARRAY_MANIPULATION_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h> 

void sortArray(char** tab, int nbr);
int checkAlpha( const char s[] );
char *toLowerWord(char *str, size_t len);
char** toLowerArray(char** x, int nbr);
void findOccurence(char** words1, char** words2, int len);
bool containsWord(char** tab, char* word);
void addWordToArray(char** arr, char* word);
void printArray(char** arr);
int removeDuplicate(char** tab, char** newTab, int len);
void read_words (FILE *f, bool options[]);

#endif
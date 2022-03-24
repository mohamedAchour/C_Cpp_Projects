#include "array_manipulation.h"

//sort array Buuble Sort algorithm
void sortArray(char** tab, int nbr){
  char temp[30];
  for(int i=0; i<nbr; i++){
    for(int j=0; j<nbr-1-i; j++){
      if(strcmp(tab[j], tab[j+1]) > 0){
        //swap tab[j] and tab[j+1]
        strcpy(temp, tab[j]);
        strcpy(tab[j], tab[j+1]);
        strcpy(tab[j+1], temp);
      }
    }
  }
}

int checkAlpha( const char s[] )
{
    unsigned char c;

    while ( ( c = *s ) && ( isalpha( c )) ) ++s;

    return *s == '\0'; 
}

char *toLowerWord(char *str, size_t len){
    char *str_l = calloc(len+1, sizeof(char));

    for (size_t i = 0; i < len; ++i) {
        str_l[i] = tolower((unsigned char)str[i]);
    }
    return str_l;
}

char** toLowerArray(char** x, int nbr){
    for(int i = 0; i < nbr;++i){
        x[i] = toLowerWord(x[i], strlen(x[i]));
    }
    return x;
}

//find the number of occurencies of words in array
//Words1: original array to search in
//words2: array containing words to searchin words1
void findOccurence(char** words1, char** words2, int len){
    int i = 0;
    int nbr_occ = 0;
    while(words2[i] != NULL){
        for(int j = 0; j < len; ++j){
            if(strcmp(toLowerWord(words2[i], sizeof(words2[i])/sizeof(char)), toLowerWord(words1[j], sizeof(words1[j])/sizeof(char))) == 0){
            ++nbr_occ;
            }  
        }
        printf("%s----> %d\n",words2[i] ,nbr_occ);
        nbr_occ = 0;

        ++i;
    }
}

bool containsWord(char** tab, char* word){
    int i = 0;
    while(tab[i] != NULL){
        if(strcmp(toLowerWord(tab[i], sizeof(tab[i])/sizeof(char)), toLowerWord(word, sizeof(word)/sizeof(char))) == 0)
        {
            return true;
        }
        ++i;
    }
    return false;
}

void addWordToArray(char** arr, char* word){
    int i = 0;
    while (arr[i] != NULL)
    {
        ++i;
    }

    arr[i] = (char*)malloc((sizeof(word)/sizeof(char) + 1)*sizeof(char));
    strcpy(arr[i], word);    
}

void printArray(char** arr){
    int i = 0;
    while (arr[i] != NULL)
    {
        printf("word: %s\n", arr[i]);
        ++i;
    }  
}

int removeDuplicate(char** tab, char** newTab, int len){
    int lenNewTab = 0;
    for(int i = 0; i < len; ++i){
        if(checkAlpha(tab[i])){
            if(containsWord(newTab, tab[i])){
                continue;
            }
            else{
                addWordToArray(newTab, tab[i]);
                ++lenNewTab;
            }
        }
    }

    return lenNewTab;
}

//read words from a given text
void read_words (FILE *f, bool options[]) {

    int nbr_of_words = 0;
    char** words = (char**)malloc(100*sizeof(char*));
    char** newTab = (char**)malloc(100*sizeof(char*));
    char word[20];
    int lenNewTab = 0;

    while (fscanf(f, " %1023s", word) == 1){
        words[nbr_of_words] = (char*)malloc(sizeof(char));
        strcpy(words[nbr_of_words], word);
        ++nbr_of_words;
    }

    lenNewTab = removeDuplicate(words, newTab, nbr_of_words);

    toLowerArray(newTab,lenNewTab);

    if(options[0]){
        sortArray(newTab, lenNewTab);
    }
    findOccurence(words, newTab, nbr_of_words);
}
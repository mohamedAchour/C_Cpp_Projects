#include "test.h"


int testaddWordToArray(){
    char** arr = (char**)malloc(10*sizeof(char*));
    char* word1 = "hello";
    char* word2 = "test";


    addWordToArray(arr, word1);

    if(strcmp(arr[0], word1)){
        printf("test failed: testaddWordToArray\n");
        return 0;
    }

    addWordToArray(arr, word2);

    if(strcmp(arr[1], word2)){
        printf("test failed: testaddWordToArray\n");
        return 0;
    }
    printf("test successfu: testaddWordToArrayl\n");
    return 1;
}   

int testsortArray(){
    char** arr = (char**)malloc(100*sizeof(char*));
    int nbr = 3;

    char* word1 = "un";
    char* word2= "hello";
    char* word3 = "trois";

    addWordToArray(arr, word1);
    addWordToArray(arr, word2);
    addWordToArray(arr, word3);

    sortArray(arr, 3);

    if(strcmp(arr[0], word2) && strcmp(arr[1], word3) && strcmp(arr[2], word1)){
        printf("test failed: testsortArray\n");
        return 0;
    }

    printf("test successfu: testsortArray\n");
    return 1;
}

int testcheckAlpha(){

    char* word1 = "Hel1";
    char* word2= "alpha";
    char* word3 = "77";

    if(checkAlpha(word1)){
        printf("test failed: testcheckAlpha\n");
        return 0;
    }

    if(!checkAlpha(word2)){
        printf("test failed: testcheckAlpha\n");
        return 0;
    }
    if(checkAlpha(word3)){
        printf("test failed: testcheckAlpha\n");
        return 0;
    }

    printf("test successful: testcheckAlpha\n");
    return 1;
}

int testtoLowerWord(){

    char* word = "HELLO";
    char* lowerWord = "hello";

    if(strcmp( toLowerWord(word, 5), lowerWord)){
        printf("test failed: testtoLowerWord\n");
        return 0;
    }
    printf("test successful: testtoLowerWord\n");
    return 1;
}

int tescontainsWord(){
    char** arr = (char**)malloc(10*sizeof(char*));
    char* word = "ici";
    
    addWordToArray(arr, "un");
    addWordToArray(arr, "trois");
    addWordToArray(arr, "ici");
    addWordToArray(arr, "tous");

    if( !containsWord(arr, word)){

        printf("test failed: tescontainsWord\n");
        return 0;
    }
    printf("test successful: tescontainsWord\n");
    return 1;
}

int test(){
    testaddWordToArray();
    testsortArray();
    testcheckAlpha();
    testtoLowerWord();
    tescontainsWord();
}

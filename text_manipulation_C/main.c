#include "array_manipulation.h"
#include "test.h"

int main(int argc, char *argv[])
{
    char *filename;
    int opt;

    //options
    //| a | n | e | test |
    bool options[4] = {false};

    if(argc < 2){
        printf("No file is given!! \
        \nUsage: ./a.out file\n");
        exit(EXIT_FAILURE);
    }
    else {
    while((opt = getopt(argc, argv, ":f:at")) != -1) 
    { 
        switch(opt) 
        { 
            case 'f': 
                filename = optarg;
                break; 
            case 'a': 
                options[0] = true;
                break; 
            case 't':  /*-test*/
                test();
                options[3] = true;
                break; 
            case '?': 
                printf("unknown option: %c\n", optopt);
                break; 
        } 
    } 
    }

    FILE *fp = fopen(filename, "r");

    if (fp == NULL)
    {
        printf("Error: could not open file %s", filename);
        return 1;
    }

    read_words(fp, options);
    fclose(fp);

    return 0;
}

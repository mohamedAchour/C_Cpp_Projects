/*********************************************************/
/****************ONE WEEK OF WOOOORK**********************/
/*********************************************************/

/*********************************************************/
/***************Querying a Document***********************/
/*******char* text ---to---> char**** document************/
/*********************************************************/
/************** |char****----> a document |***************/
/************** |char*** ----> a paragraph|***************/
/************** |char**  ----> a sentence |***************/
/************** |char*   ----> a word     |***************/
/*********************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<assert.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

char* kth_word_in_mth_sentence_of_nth_paragraph(char**** document, int k, int m, int n) {
    char* word=NULL;
    word=(char*)malloc((1+strlen(document[n-1][m-1][k-1]))*sizeof(char)); 
    memset(word,0,sizeof(char)*(1+strlen(document[n-1][m-1][k-1])));
    word=document[n-1][m-1][k-1];
    return word;
}

char** kth_sentence_in_mth_paragraph(char**** document, int k, int m) { 
    char** kth_sent_mth_parag=NULL;
    
    kth_sent_mth_parag=(char**)malloc(sizeof(document[m-1][k-1]));
    kth_sent_mth_parag=document[m-1][k-1];
    return kth_sent_mth_parag;
}

char*** kth_paragraph(char**** document, int k) {
    
    char*** kth_par=NULL;
    kth_par=(char***)malloc(sizeof(document[k-1]));
    kth_par=document[k-1];
    
    return kth_par;
}
char**** get_document(char* text) {
    char**** returnDocument=NULL;

    unsigned int w_count=1,s_count=1,p_count=1;
    char *ch=text;
    unsigned int char_count=0;
    unsigned int text_counter=0;

    
    returnDocument=(char****)malloc(sizeof(char***));
    returnDocument[p_count-1]=(char***)malloc(sizeof(char**));
    returnDocument[p_count-1][s_count-1]=(char**)malloc(sizeof(char*));
    returnDocument[p_count-1][s_count-1][w_count-1]=(char*)malloc(sizeof(char));
    
    
    while(text_counter<strlen(text))
    {
        if((*(ch+text_counter))!='\n')
        {
            if(*(ch+text_counter)!='.')
            {
                if(*(ch+text_counter)!=' ')
                {
                    char_count++;
                    returnDocument[p_count-1][s_count-1][w_count-1]=(char*)realloc(returnDocument[p_count-1][s_count-1][w_count-1],char_count*sizeof(char));
                    returnDocument[p_count-1][s_count-1][w_count-1][char_count-1]=*(ch+text_counter);
                }
                else
                {
                    returnDocument[p_count-1][s_count-1][w_count-1]=(char*)realloc(returnDocument[p_count-1][s_count-1][w_count-1],(char_count+1)*sizeof(char));
                    returnDocument[p_count-1][s_count-1][w_count-1][char_count]='\0';
                    char_count=0;
                    w_count++;

                    returnDocument[p_count-1][s_count-1]=(char**)realloc(returnDocument[p_count-1][s_count-1],w_count*sizeof(char*));
                    returnDocument[p_count-1][s_count-1][w_count-1]=(char*)malloc(sizeof(char));
                    
                }
            }     
            else
            {
                returnDocument[p_count-1][s_count-1][w_count-1]=(char*)realloc(returnDocument[p_count-1][s_count-1][w_count-1],(char_count+1)*sizeof(char));
                    returnDocument[p_count-1][s_count-1][w_count-1][char_count]='\0';
                    char_count=0;
                w_count=1;   
                char_count=0;  
                s_count++;
                returnDocument[p_count-1]=(char***)realloc(returnDocument[p_count-1],s_count*sizeof(char**));  
                returnDocument[p_count-1][s_count-1]=(char**)malloc(sizeof(char*));   
                returnDocument[p_count-1][s_count-1][w_count-1]=(char*)malloc(sizeof(char)); 
            }
        }

        else
        {
            w_count=1;   
            char_count=0;  
            s_count=1;
            p_count++;

            returnDocument=(char****)realloc(returnDocument,p_count*sizeof(char***));
            returnDocument[p_count-1]=(char***)malloc(sizeof(char**));  
            returnDocument[p_count-1][s_count-1]=(char**)malloc(sizeof(char*));
            returnDocument[p_count-1][s_count-1][w_count-1]=(char*)malloc(sizeof(char));

        }

        text_counter++;
    }
    return returnDocument;
}

char* get_input_text() {	
    int paragraph_count;
    scanf("%d", &paragraph_count);

    char p[MAX_PARAGRAPHS][MAX_CHARACTERS], doc[MAX_CHARACTERS];
    memset(doc, 0, sizeof(doc));
    getchar();
    for (int i = 0; i < paragraph_count; i++) {
        scanf("%[^\n]%*c", p[i]);
        strcat(doc, p[i]);
        if (i != paragraph_count - 1)
            strcat(doc, "\n");
    }

    char* returnDoc = (char*)malloc((strlen (doc)+1) * (sizeof(char)));
    strcpy(returnDoc, doc);
    return returnDoc;
}

void print_word(char* word) {
    printf("%s", word);
}

void print_sentence(char** sentence) {
    int word_count;
    scanf("%d", &word_count);
    for(int i = 0; i < word_count; i++){
        printf("%s", sentence[i]);
        if( i != word_count - 1)
            printf(" ");
    }
} 

void print_paragraph(char*** paragraph) {
    int sentence_count;
    scanf("%d", &sentence_count);
    for (int i = 0; i < sentence_count; i++) {
        print_sentence(*(paragraph + i));
        printf(".");
    }
}

int main() 
{
    char* text = get_input_text();
    char**** document = get_document(text);

    /*****some tests******/
    char*** k_th_paragraph=kth_paragraph(document,1);
    char**  k_th_sent_mth_p=kth_sentence_in_mth_paragraph(document,1,1);
    char*   k_th_w_mth_s_nth_p=kth_word_in_mth_sentence_of_nth_paragraph(document,1,1,1);

    printf("%s\n",k_th_paragraph[0][0]);
    printf("%s\n",k_th_sent_mth_p[0]);
    printf("%s\n",k_th_w_mth_s_nth_p);

    return 0;

}
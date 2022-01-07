#include<stdio.h>
#include<stdlib.h>
#include<string.h>


/*maximum lenght ---> string for gets()*/
#define max_len 10

void clear_buff()
{
    /*clear the buffer*/
    //while (getchar()!='\n');

    /*for more security*/
    int c = 0;
    
    while (c != '\n' && c != EOF)
    {
        c = getchar();
    }
    
}

int read_str(char* my_str, int len)
{
    int  i=0;
    char *ch=NULL;
    if(fgets(my_str,len,stdin)!=NULL)
    {       
        ch = strchr(my_str, '\n');
        if (ch != NULL)
        {
            /*in this case we have reached the end of the stirng so there is
            no need to clear buffer (gets() already did it!!)*/
            *ch = '\0';
        }

        else
        {
            /*here we have to clear the buffer*/
            clear_buff();
        }
           
        return 0;
    }
    else { 
            /*yes also in this case we have to cleare the buffer for more security*/
            clear_buff();
            return -1;    
         }
}

long read_long()
{
    long nbr=0;
    char mystrp[10]="";

    if(read_str(mystrp,max_len)==0)
    {
        /*for float see ---> strtod*/
        nbr=strtol(mystrp,NULL,10);
        return nbr;
    }    

    else
    {
        exit(EXIT_FAILURE);
    }
}

int main(int argc, char *argv[])
{
    char *my_str=NULL;  //static declaration--> my_str[?] gives segfault !!!
    char *ch=0;
    long nb=0;

  
    /*secure text entry*/
    /*dynmaic allocation to avoid segfault when calling read_str() function*/
    
    my_str=(char*)malloc((sizeof(char))*max_len);
    printf("string : ");
    read_str(my_str, max_len);
    printf("you've typed : %s --> yeeees\n",my_str);
    
    /*get a number*/
    printf("number = ");
    nb=read_long();
    printf("you've typed : %ld\n",nb);
    printf("This file is compiled the: %s.\n",__DATE__);      
    return 0;
}
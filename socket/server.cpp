#include<iostream>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<stdlib.h>

using namespace std;

int main()
{
    int client, server;
    int portNum= 1501;
    bool isExit=false;
    int bufsize=1024;
    char buffer[bufsize];

    struct sockaddr_in server_addr;
    socklen_t size; 

    client=socket(AF_INET, SOCK_STREAM, 0);

    if (client<0)
    {
        cout<<"error establishing connection"<< endl;
        exit(1);
    }

    cout<<"server conenction created ..." <<endl;
    server_addr.sin_family=AF_INET;
    server_addr.sin_addr.s_addr=htons(INADDR_ANY);
    server_addr.sin_port=htons(portNum);

    //binding socket
    if (bind(client, (struct sockaddr*)&server_addr, sizeof(server_addr))<0)
    {
        cout<<"error binding socket..."<<endl;
        exit(1);
    }
    size=sizeof(server_addr);
    cout<<"looking for clients..."<<endl;

    //listening socket
    listen(client,1);

    //accept client
    server=accept(client, (struct sockaddr*)&server_addr,&size);

    if(server<0)
    {
        cout<<"Error on accepting ..."<<endl;
        exit(1);
    }

    while(server>0)
    {
        strcpy(buffer, "server connected...\n");
        send(server,buffer,bufsize,0);
        cout<<"conencted with client ..."<<endl;
        cout<<"enter # to end the conenction" <<endl;
        cout<<"client: ";
        do
        {
            recv(server,buffer,bufsize,0);
            cout<<"buffer"<< " ";
            if(*buffer=='#')
            {
                *buffer='*';
                isExit= true;
            }
        } while(*buffer != '*');

        do
        {
            cout<<"\nServer: ";
            do
            {
                cin>>buffer;
                send(server,buffer,bufsize,0);
                if(*buffer=='#')
                {
                    send(server, buffer, bufsize,0);
                    *buffer='*';
                    isExit=true;
                }
            } while (*buffer!= '*');
            cout<<"client: ";
            do 
            {
                recv(server,buffer,bufsize,0);
                cout<<buffer<<"";
                if(*buffer=='#')
                {
                    *buffer='*';
                    isExit=true;
                }
            } while(*buffer!='*');
        } while(!isExit);

        cout<<"connection terminated ..."<< endl;
        cout<<"Goodbye...."<<endl;
        isExit=false;
        exit(1);
    }
    close(client);
    
    return 0;
}
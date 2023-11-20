#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#define MAX_NAME 100
#define MAX_DATA 1000

//previously used define statements, subject to change
#define PORT 8080
#define MAXLINE 1024
#define SERVERADDRESS "192.168.1.1"

typedef struct message
{
    unsigned int type;
    unsigned int size;
    unsigned char source[MAX_NAME];
    unsigned char data[MAX_DATA];
}Message;

int main(){
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    if(sockfd == -1){
        printf("socket creation failed");
        exit(0);
    }
    else{
        printf("socket successfully created");
    }
    bzero(&serveraddr, sizeof(serveraddr));

    //assign IP, PORT
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(PORT);
    
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>

#include <netdb.h>
#include <netinet/in.h>

#define MAX_NAME 100
#define MAX_DATA 1000

#define SA struct sockaddr

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

void chat(int sockfd){
    char buff[MAX_DATA];
    int n;
    for(;;){
        bzero(buff, sizeof(buff));
        printf("Enter string to send: ");
        n = 0;
        while((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));
        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("Server sent: %s\n", buff);
        if((strncmp(buff, "exit", f)) == 0){
            printf("Client exit\n");
            break;
        }
    }
}

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
    bzero(&servaddr, sizeof(servaddr));

    //assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0){
        printf("failed to establish connection with server");
        exit(0);
    }
    else{
        printf("established connection with server");
    }

    //function for chat
    chat(sockfd);

    //closing socket
    close(sockfd);

}
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

char *extractSubstring(const char *inputString, int start, int end) {
    if (inputString == NULL || start < 0 || end < start || end >= strlen(inputString)) {
        return NULL; // Invalid input
    }

    // Calculate the length of the substring
    int length = end - start + 1;

    // Allocate memory for the substring
    char *substring = (char *)malloc(length + 1); // +1 for null-terminator

    if (substring == NULL) {
        return NULL; // Memory allocation failed
    }

    // Copy characters from the original string to the substring
    for (int i = 0; i < length; i++) {
        substring[i] = inputString[start + i];
    }

    // Null-terminate the substring
    substring[length] = '\0';

    return substring;
}

//convert raw user input text into Message struct
Message
textToMessage (char *text, char *source)
{
  //integer defining message type
  int type;
  int args = 0;
  //choose what type of message it is
  if (strncmp (text, "/login", 6) == 0)
    {
      type = 0;
      args = 3;
    }
  else if (strncmp (text, "/logout", 7) == 0)
    {
      type = 3;
    }
  else if (strncmp (text, "/joinsession", 12) == 0)
    {
      type = 4;
      args = 2;
    }
  else if (strncmp (text, "/leavesession", 13) == 0)
    {
      type = 7;
    }
  else if (strncmp (text, "/createsession", 14) == 0)
    {
      type = 8;
      args = 2;
    }
  else if (strncmp (text, "/list", 5) == 0)
    {
      type = 11;
    }
  else if (strncmp (text, "/quit", 5) == 0)
    {
      type = 3;
    }
  else
    {
      type = 10;
    }


  Message ret;

  ret.type = type;
  
  strcpy(ret.data, "");

  if (args != 0){
    char *token = strtok (text, " ");

    for (int i = 0; i < args; i++){
    	  if (i == 0){
    	      //first argument, command so do nothing
    	    }
    	  else{
    	      strcat (ret.data, token);
    	      strcat(ret.data, " ");
    	    }
    	  token = strtok (NULL, " ");
    }
  }
  else if(type == 10){
    strcpy(ret.data, text);
  }
  ret.size = sizeof(ret.data);
  strcpy(ret.source, source);
  
  return ret;
}

//convert message struct to string with ":" to send with TCP
char *
messageToString (Message message0)
{
    int strSize[] = { 0, 0 };
    double temp;
    //if message is of type 0, special case since we can't count digits in it
    if(message0.type != 0){
        temp = (double) message0.type;
        while (temp >= 1){
            temp /= 10;
            strSize[0]++;
        }
  }
  else{
      strSize[0] = 1;
  }
  
    temp = (double) message0.size;
    while (temp >= 1)
    {
      temp /= 10;
      strSize[1]++;
    }

  char *str =
    (char *)
    malloc ((strSize[0] + strSize[1] + strlen (message0.source) +
	     strlen (message0.data) + 3) * sizeof (char));

  char buffer0[strSize[0]];
  char buffer1[strSize[1]];

  snprintf (buffer0, strSize[0] + 1, "%d", message0.type);
  snprintf (buffer1, strSize[1] + 1, "%d", message0.size);

  strcat (str, buffer0);
  strcat (str, ":");
  strcat (str, buffer1);
  strcat (str, ":");
  strcat (str, message0.source);
  strcat (str, ":");
  strcat (str, message0.data);
  strcat (str, "\0");

  return str;
}

void chat(int sockfd){
    char buff[sizeof(Message)];
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
        if((strncmp(buff, "exit", 4)) == 0){
            printf("Client exit\n");
            break;
        }
    }
}

int main(){
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("socket creation failed\n");
        exit(0);
    }
    else{
        printf("socket successfully created\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    //assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0){
        printf("failed to establish connection with server\n");
        exit(0);
    }
    else{
        printf("established connection with server\n");
    }

    //function for chat
    chat(sockfd);

    //closing socket
    close(sockfd);

}
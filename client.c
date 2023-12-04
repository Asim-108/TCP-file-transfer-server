#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

#include <netdb.h>
#include <netinet/in.h>

#define MAX_NAME 100
#define MAX_DATA 1000

#define SA struct sockaddr

//previously used define statements, subject to change
#define PORT 8080
#define MAXLINE 1024
#define SERVERADDRESS "192.168.1.1"

// /login Asim test 128.100.13.54 8080

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

//clients and server
// Message* stringToMessage(char* str){
//     char* token = strtok(str, ":");

//     int count = 3;
    
//     //if type has valid data field
//     if (strncmp (token, "0", 1) == 0 || strncmp (token, "2", 1) == 0
//     || strncmp (token, "4", 1) == 0 || strncmp (token, "5", 1) == 0
//     || strncmp (token, "6", 1) == 0 || strncmp (token, "9", 1) == 0
//     || strncmp (token, "10", 1) == 0 || strncmp (token, "12", 1) == 0){
//         count = 4;
//     }
    
//     Message* message0 = (Message*) malloc(sizeof(Message));
    
//     for(int i = 0; i < count; i++){
//         if(i == 0){
//             message0->type = atoi(token);
//         }
//         else if(i == 1){
//             message0->size = atoi(token);
//         }
//         else if(i == 2){
//             //initialize memory
//             strcpy(message0->source, "");
//             strcpy(message0->source, token);
//         }
//         else{
//             strcpy(message0->source, "");
//             strcpy(message0->data, token);
//         }
//         token = strtok(NULL, ":");
//     }
    
//     return message0;
// }

Message stringToMessage(char* str){
  char* token = strtok(str, ":");

  int count = 3;
  
  //if type has valid data field
  if (strncmp (token, "0", 1) == 0 || strncmp (token, "2", 1) == 0
  || strncmp (token, "4", 1) == 0 || strncmp (token, "5", 1) == 0
  || strncmp (token, "6", 1) == 0 || strncmp (token, "9", 1) == 0
  || strncmp (token, "10", 1) == 0 || strncmp (token, "12", 1) == 0){
    count = 4;
  }
  
  Message message0;
  
  for(int i = 0; i < count; i++){
    if(i == 0){
      message0.type = atoi(token);
    }
    else if(i == 1){
      message0.size = atoi(token);
    }
    else if(i == 2){
      //initialize memory
      strcpy(message0.source, "");
      strcpy(message0.source, token);
    }
    else{
      strcpy(message0.data, "");
      strcpy(message0.data, token);
    }
    token = strtok(NULL, ":");
  }
  
  return message0;
}



char* messageToText(Message message0){
    if (message0.type != 0 || message0.type != 2 || message0.type != 4
    || message0.type != 5 || message0.type != 6 || message0.type != 9
    || message0.type != 10 || message0.type != 12){
        //no message to display, return
        return;
    }
    
    char* text = message0.data;
    return text;
}

void chat(int sockfd, char* source){
  char buff[sizeof(Message)];
  int n;
  char sentence[1000];
  char *tokens[50];
  int tokencount = 0;

  for(;;){
    bzero(buff, sizeof(buff));
    printf("Enter message or command: ");
    n = 0;

    // while((buff[n++] = getchar()) != '\n')
    //     ;

    fgets(sentence, sizeof(sentence), stdin);

    char text[sizeof(sentence)];
    strcpy(text, sentence);

    //splitStringBySpace(sentence, tokens, &tokencount);
    tokencount = 0;
    char *token = strtok(sentence, " ");
    // Iterate through the tokens and print them
    while (token != NULL) {
      // tokens[tokencount] = token; 
      strcpy(tokens[tokencount], token);
      
      (tokencount++);
      token = strtok(NULL, " ");
    }
    for(int i =0; i < tokencount; i++){
      printf("%s \n",tokens[i]);
    }

    if(strncmp(token[0], "/login", 6) == 0){
      printf("already logged in\n");
      continue;
    }
    //converting input text into Message struct
    Message messageToSend = textToMessage(text, source);
    //converting message struct into string with ":" seperations to send
    char* sendBuff = messageToString(messageToSend);
    //send to the server the message
    write(sockfd, sendBuff, sizeof(sendBuff));
    //freeing memory allocated for buffer to send
    free(sendBuff);
    bzero(buff, sizeof(buff));
    //receive reply from server
    read(sockfd, buff, sizeof(buff));

    //different response printed based on type of message server sent
    Message messageReceived = stringToMessage(buff);
    if(messageReceived.type == 3){
      printf("logout/exit from server\n");
      break;
    }
    else if(messageReceived.type == 5){
      printf("successfully joined session %s\n", messageReceived.data);
    }
    else if(messageReceived.type == 6){
      printf("could not join the session %s\n", messageReceived.data);
    }
    else if(messageReceived.type == 9){
      printf("successfully created session %s\n", messageReceived.data);
    }
    else if(messageReceived.type == 10){
      //message received from a host, check if it is for us

      char* token = strtok(messageReceived.data, "~");
      char* prev;
      bool displayMessage = false;

      while(token != NULL){
        if(strcmp(source, token) == 0){
          displayMessage = true;
        }
        prev = token;
        token = strtok(NULL, "~");
      }
      if(displayMessage){
        printf("%s: %s\n", messageReceived.source, messageReceived.data);
      }
    }
    else if(messageReceived.type == 11){
      printf("list of sessions:\n %s\n", messageReceived.data);
    }
    else{
      printf("UNEXPECTED MESSAGE TYPE FROM SERVER, ERROR");
    }


    // printf("Server sent: %s\n", buff);
    // if((strncmp(buff, "exit", 4)) == 0){
    //   printf("Client exit\n");
    //   break;
    // }
  }
}

int main(){
  int sockfd, connfd;
  struct sockaddr_in servaddr, cli;

  char input[1000];
  char tokens[50][50];
  int tokencount = 0;

  bool loggedIn = false;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd == -1){
    printf("socket creation failed\n");
    exit(0);
  }
  else{
    printf("socket successfully created\n");
  }
  bzero(&servaddr, sizeof(servaddr));

  printf("login to a server with /login\n");

  fgets(input, sizeof(input), stdin);
  char inputCopy[sizeof(input)];
  strcpy(inputCopy, input);

  while(!loggedIn){
    //splitStringBySpace(sentence, tokens, &tokencount);
    char *token = strtok(input, " ");
    // Iterate through the tokens and print them
    while (token != NULL) {
      
      // tokens[tokencount] = token; 
      strcpy(tokens[tokencount], token);
      
      (tokencount++);
      token = strtok(NULL, " ");
    }

    //if not login command
    if(strcmp(tokens[0], "/login") != 0){
      printf("Invalid input, please login with /login\n");

      tokencount = 0;
      bzero(tokens, sizeof(tokens));
      fgets(input, sizeof(input), stdin);
      char inputCopy[sizeof(input)];
      strcpy(inputCopy, input);
    }
    else if(tokencount != 5){
      printf("Wrong number of arguments for /login command\n");

      tokencount = 0;
      bzero(tokens, sizeof(tokens));
      fgets(input, sizeof(input), stdin);
      char inputCopy[sizeof(input)];
      strcpy(inputCopy, input);
    }
    else{
      loggedIn = 1;
    }
  }
  

  //assign server IP, PORT using what client entered
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = inet_addr(tokens[3]);
  servaddr.sin_port = htons(atoi(tokens[4]));

  if(connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0){
    printf("failed to establish connection with server\n");
    exit(0);
  }
  else{
    printf("established connection with server\n");
  }

  //try to login to the server

  Message loginAttempt = textToMessage(inputCopy, tokens[1]);
  char* sendBuff = messageToString(loginAttempt);

  write(sockfd, sendBuff, sizeof(sendBuff));
  // free(sendBuff);

  char buff[sizeof(Message)];
  bzero(buff, sizeof(buff));
  read(sockfd, buff, sizeof(buff));

  Message serverReply = stringToMessage(buff);

  if(serverReply.type == 1){
    printf("logged in successfully as: %s", tokens[1]);
    char source[sizeof(tokens[1])];
    strcpy(source, tokens[1]);
    chat(sockfd, source);
  }
  else{
    //not able to log in, server will tell why
    printf("%s", serverReply.data);
  }

  //finished chatting OR unable to login at this point, exit program

  //closing socket
  close(sockfd);

  return 0;
}

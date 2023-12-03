// Driver code
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

#define MAX_NAME 100
#define MAX_DATA 1000

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

int main(int argc, char* argv[]) {

    char tempsentence[1000];
    char sentence[1000];
    char *tokens[50];
    int tokencount = 0;
    
    //Message
    int type;
    int size;
    char source[100];
    char data[100];
    char clientid[100];
    strcpy(clientid, "-1");
    bool exit = false;
    while(!exit){
        printf("Enter a sentence: ");
        fgets(sentence, sizeof(sentence), stdin);

        char text[sizeof(sentence)];
        strcpy(text, sentence);

        //splitStringBySpace(sentence, tokens, &tokencount);
        tokencount = 0;
        char *token = strtok(sentence, " ");
        // Iterate through the tokens and print them
        while (token != NULL) {
            tokens[tokencount] = token; 
            
            (tokencount++);
            token = strtok(NULL, " ");
        }
        for(int i =0; i < tokencount; i++){
            printf("%s \n",tokens[i]);
        }

        if(strncmp(token[0], "/login", 6) == 0){
            //extract IP and port from token [3] and [4]
        }
        Message message0;
        message0 = textToMessage(text, token[1]);

        char* str = messageToString(message0);

        // sendto(str);

        free(str);

        // recvfrom(n)







        
        // if(!strcmp(tokens[0],"/login")){
        //     if(strcmp(clientid, "-1")){
        //         printf("logged in already");
        //     }
        //     else if(tokencount != 5){
        //         printf("wrong num of arguments");
        //     }
        //     else{
        //         type = 0;
        
        //         //Client id = tokens[1];
        //         //client password = tokens[2];

        //         //After LOGIN ACK
        //         //strcpy(clientid, tokens[1]);

        //         printf("login");
        //     }
            
        // }
        // else if(!strcmp(extractSubstring(sentence, 0, 6), "/logout")){
        //     if(!strcmp(clientid, "-1")){
        //         printf("not logged in");
        //     }
        //     else if(tokencount != 1){
        //         printf("wrong num of arguments");
        //     }
        //     else{
        //         printf("logout");
            
            
        //         type = 3;
        //         //assume exit
        //     }
            

        // }
        // else if(!strcmp(tokens[0],  "/joinsession")){
        //     if(tokencount != 2){
        //         printf("wrong num of arguments");
        //     }
        //     else{
        //         printf("join Session");
        //         type = 4;
        //         strcpy(data, tokens[1]);
        //         //Send message to 
        //         //
        //     }
            
        // }
        // else if(!strcmp(extractSubstring(sentence, 0, 12),  "/leavesession")){
        //     if(tokencount != 1){
        //         printf("wrong num of arguments");
        //     }
        //     else{
        //         printf("leave session");
        //         type = 7;
        //         //
        //     }
            
        // }
        // else if(!strcmp(tokens[0],  "/createsession")){
        //     if(tokencount != 2){
        //         printf("wrong num of arguments");
        //     }
        //     else{
        //         printf("create session");
        //         type = 8;
        //         //
        //     }
            
        // }
        // else if(!strcmp(extractSubstring(sentence, 0, 4),  "/list")){
        //     if(tokencount != 1){
        //         printf("wrong num of arguments");
        //     }
        //     else{
        //         type = 11;
        //         printf("list");
                
        //         strcpy(source, clientid);
        //         //Server needs to check which clients are in there session and returns the related clients
        //         //And list all the related Clients
        //     }
        // }
        // else if(!strcmp(extractSubstring(sentence, 0, 4),  "/quit")){
        //     if(tokencount != 1){
        //         printf("wrong num of arguments");
        //     }
        //     else{
        //         printf("quit");
        //         type = 3;
        //         //Send message to server to disconnect this client to server
        //         //Send client and quit
        //         exit = true;
        //     } 
        // }
        // else if (tokencount == 1){
        //     //Message
        //     type = 10;
        //     printf("this is text");
        // }
        // else{
        //     printf("error");
        // }
    }




}




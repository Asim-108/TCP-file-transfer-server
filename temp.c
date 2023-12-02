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

        
        if(!strcmp(tokens[0],"/login")){
            if(strcmp(clientid, "-1")){
                printf("logged in already");
            }
            else if(tokencount != 5){
                printf("wrong num of arguments");
            }
            else{
                type = 0;
        
                //Client id = tokens[1];
                //client password = tokens[2];

                //After LOGIN ACK
                //strcpy(clientid, tokens[1]);

                printf("login");
            }
            
        }
        else if(!strcmp(extractSubstring(sentence, 0, 6), "/logout")){
            if(!strcmp(clientid, "-1")){
                printf("not logged in");
            }
            else if(tokencount != 1){
                printf("wrong num of arguments");
            }
            else{
                printf("logout");
            
            
                type = 3;
                //assume exit
            }
            

        }
        else if(!strcmp(tokens[0],  "/joinsession")){
            if(tokencount != 2){
                printf("wrong num of arguments");
            }
            else{
                printf("join Session");
                type = 4;
                strcpy(data, tokens[1]);
                //Send message to 
                //
            }
            
        }
        else if(!strcmp(extractSubstring(sentence, 0, 12),  "/leavesession")){
            if(tokencount != 1){
                printf("wrong num of arguments");
            }
            else{
                printf("leave session");
                type = 7;
                //
            }
            
        }
        else if(!strcmp(tokens[0],  "/createsession")){
            if(tokencount != 2){
                printf("wrong num of arguments");
            }
            else{
                printf("create session");
                type = 8;
                //
            }
            
        }
        else if(!strcmp(extractSubstring(sentence, 0, 4),  "/list")){
            if(tokencount != 1){
                printf("wrong num of arguments");
            }
            else{
                type = 11;
                printf("list");
                
                strcpy(source, clientid);
                //Server needs to check which clients are in there session and returns the related clients
                //And list all the related Clients
            }
        }
        else if(!strcmp(extractSubstring(sentence, 0, 4),  "/quit")){
            if(tokencount != 1){
                printf("wrong num of arguments");
            }
            else{
                printf("quit");
                type = 3;
                //Send message to server to disconnect this client to server
                //Send client and quit
                exit = true;
            } 
        }
        else if (tokencount == 1){
            //Message
            type = 10;
            printf("this is text");
        }
        else{
            printf("error");
        }
    }




}




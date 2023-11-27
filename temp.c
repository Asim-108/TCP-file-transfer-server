// Driver code
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

void splitStringBySpace( char *inputString, char *tokens[],int *tokenCount) {
    // Make a copy of the input string because strtok modifies the string
    *tokenCount = 0;
    char *token = strtok(inputString, " ");

    // Iterate through the tokens and print them
    while (token != NULL) {
        tokens[*tokenCount] = token; 
        printf(token);
        (*tokenCount++);
        token = strtok(NULL, " ");
    }
}



int main(int argc, char* argv[]) {
    int hi = 0;
    char string[1000];
    char tempsentence[100];
    char sentence[100];
    char *tokens[50];
    int tokencount = 0;
    
    //Message
    int type;
    int size;
    char source[100];
    char data[100];
    char clientid[100];

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

    if(strcpy(extractSubstring(sentence, 0, 6), tempsentence) && strcmp(tempsentence,  "/login " && tokencount == 5)){
        type = 0;
       
        //Client id = tokens[1];
        //client password = tokens[2];

        //After LOGIN ACK
        //strcpy(clientid, tokens[1]);

        printf("login");
    }
    else if(strcpy(extractSubstring(sentence, 0, 7), tempsentence) && strcmp(tempsentence,  "/logout ") && tokencount == 1){
        printf("logout");
        type = 3;
        //assume exit

    }
    else if(strcpy(extractSubstring(sentence, 0, 12), tempsentence) && strcmp(tempsentence,  "/joinsession ")&& tokencount == 2){
        printf("join Session");
        type = 4;
        strcpy(data, tokens[1]);
        //Send message to 
        //
    }
    else if(strcpy(extractSubstring(sentence, 0, 13), tempsentence) && strcmp(tempsentence,  "/leavesession ")&& tokencount == 1){
        printf("leave session");
        type = 7;
        //
    }
    else if(strcpy(extractSubstring(sentence, 0, 14), tempsentence) && strcmp(tempsentence,  "/createsession ")&& tokencount == 2){
        printf("create session");
        type = 8;
        //
    }
    else if(strcpy(extractSubstring(sentence, 0, 5), tempsentence) && strcmp(tempsentence,  "/list ")&& tokencount == 1){
        type = 11;
        printf("list");
        
        strcpy(source, clientid)
        //Server needs to check which clients are in there session and returns the related clients
        //And list all the related Clients
    }
    else if(strcpy(extractSubstring(sentence, 0, 5), tempsentence) && strcmp(tempsentence,  "/quit ")&& tokencount == 1){
        printf("quit");
        type = 3;
        //Send message to server to disconnect this client to server
        //Send client and quit
        exit(1);
    }
    else if (tokencount == 1){
        type = 10;
        printf("this is text");
    }

    



}




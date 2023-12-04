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

#define SA struct sockaddr

//previously used define statements, subject to change
#define PORT 8080
#define MAXLINE 1024
#define SERVERADDRESS "192.168.1.1"

// Define the structure for a node in the linked list
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>

struct Node {
    char* data;  // Change the data type to char*
    struct Node* next;
};

struct MultiLinkedList {
    struct Node** lists;
    char** names;
    int numLists;
};

void initalize_sessions(struct MultiLinkedList* multiList, int numLists) {
    multiList->numLists = numLists;
    multiList->lists = (struct Node**)malloc(numLists * sizeof(struct Node*));
    multiList->names = (char**)malloc(numLists * sizeof(char*));

    for (int i = 0; i < numLists; ++i) {
        multiList->lists[i] = NULL;
        char buffer[12];
        sprintf(buffer, "%d", i + 1);
        multiList->names[i] = strdup(buffer);
    }
}

void addNode(struct Node** head, const char* data) {
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    newNode->data = strdup(data);  // Copy the string
    newNode->next = *head;
    *head = newNode;
}


bool  deletePerson_from_session(struct MultiLinkedList* multiList, const char* listName, const char* data) {
    for (int i = 0; i < multiList->numLists; ++i) {
        if (strcmp(multiList->names[i], listName) == 0) {
            // Found the named list, proceed to delete node
            struct Node* current = multiList->lists[i];
            struct Node* prev = NULL;

            while (current != NULL) {
                if (strcmp(current->data, data) == 0) {
                    // Node with the specified data found, unlink and free
                    if (prev == NULL) {
                        // Node to be deleted is the head
                        multiList->lists[i] = current->next;
                    } else {
                        // Node to be deleted is in the middle or end
                        prev->next = current->next;
                    }

                    free(current->data);
                    free(current);
                    return true;
                }

                prev = current;
                current = current->next;
            }

            // Data not found in the list
            printf("Error: Node with data '%s' not found in the list '%s'\n", data, listName);
            return false;
        }
    }

    // Named list not found
    printf("Error: List with name '%s' not found\n", listName);
}

void create_session(struct MultiLinkedList* multiList, const char* name) {
    multiList->numLists++;
    multiList->lists = (struct Node**)realloc(multiList->lists, multiList->numLists * sizeof(struct Node*));
    multiList->names = (char**)realloc(multiList->names, multiList->numLists * sizeof(char*));
    multiList->lists[multiList->numLists - 1] = NULL;
    multiList->names[multiList->numLists - 1] = strdup(name);
}

bool Join_session(struct MultiLinkedList* multiList, const char* listName, const char* data) {
    for (int i = 0; i < multiList->numLists; ++i) {
        if (strcmp(multiList->names[i], listName) == 0) {
            // Found the named list, add node to it
            addNode(&multiList->lists[i], data);
            return true;
        }
    }
    // If the named list is not found, you can handle the error or provide a message
    printf("Error: List with name '%s' not found\n", listName);
    return false;
}

// void displayList(struct Node* head) {
//     while (head != NULL) {
//         printf("%s -> ", head->data);
//         head = head->next;
//     }
//     printf("NULL\n");
// }

int isNameInList(struct Node* head, const char* name) {
    while (head != NULL) {
        if (strcmp(head->data, name) == 0) {
            // Name found in the list
            return 1;
        }
        head = head->next;
    }
    // Name not found in the list
    return 0;
}

// Function to get the name of the session where a specific name is present
char* getSessionName(struct MultiLinkedList* multiList, const char* targetName) {
    for (int i = 0; i < multiList->numLists; ++i) {
        if (isNameInList(multiList->lists[i], targetName)) {
            // Target name is present in this session
            return multiList->names[i];
        }
    }

    // Name not found in any session
    return NULL;
}

char* getNamesInList(struct Node* head) {
    if (head == NULL) {
        // Return NULL if the list is empty
        return NULL;
    }

    // Calculate the total length of the result string
    size_t totalLength = 0;
    struct Node* current = head;
    while (current != NULL) {
        totalLength += strlen(current->data) + 2;  // 2 for separating characters (~ and space)
        current = current->next;
    }

    // Allocate memory for the result string
    char* namesString = (char*)malloc(totalLength);
    if (namesString == NULL) {
        // Memory allocation failure
        return NULL;
    }

    namesString[0] = '\0';  // Initialize the string as empty

    // Concatenate names to the result string
    current = head;
    while (current != NULL) {
        strcat(namesString, current->data);
        strcat(namesString, "~");  // Use any separator you prefer
        current = current->next;
    }

    // Remove the trailing separator and return the result string
    namesString[strlen(namesString) - 2] = '\0';
    return namesString;
}

// Function to get names in a specified session
char* getNamesInSession(struct MultiLinkedList* multiList, const char* sessionName) {
    for (int i = 0; i < multiList->numLists; ++i) {
        if (strcmp(multiList->names[i], sessionName) == 0) {
            // Found the named session, use the existing function to get all names
            return getNamesInList(multiList->lists[i]);
        }
    }

    // Named session not found
    printf("Error: Session with name '%s' not found\n", sessionName);
    return NULL;
}


char* list_of_session(struct MultiLinkedList* multiList) {
    char* result = (char*)malloc(1);  // Start with an empty string
    result[0] = '\0';

    for (int i = 0; i < multiList->numLists; ++i) {
        char temp[256];  // Adjust the size based on your needs
        snprintf(temp, sizeof(temp), "List %s: ", multiList->names[i]);

        // Concatenate the list representation to the result string
        result = (char*)realloc(result, strlen(result) + strlen(temp) + 1);
        strcat(result, temp);

        struct Node* current = multiList->lists[i];
        while (current != NULL) {
            char nodeData[256];  // Adjust the size based on your needs
            snprintf(nodeData, sizeof(nodeData), "%s -> ", current->data);

            // Concatenate the node data to the result string
            result = (char*)realloc(result, strlen(result) + strlen(nodeData) + 1);
            strcat(result, nodeData);

            current = current->next;
        }

        // Concatenate "NULL" to indicate the end of the list
        result = (char*)realloc(result, strlen(result) + 6 + 1);
        strcat(result, "NULL\n");
    }

    return result;
}



void freeList(struct Node* head) {
    while (head != NULL) {
        struct Node* temp = head;
        head = head->next;
        free(temp->data);  // Free the string
        free(temp);
    }
}

void free_sessions(struct MultiLinkedList* multiList) {
    for (int i = 0; i < multiList->numLists; ++i) {
        freeList(multiList->lists[i]);
        free(multiList->names[i]);
    }
    free(multiList->lists);
    free(multiList->names);
}



typedef struct message
{
    unsigned int type;
    unsigned int size;
    unsigned char source[MAX_NAME];
    unsigned char data[MAX_DATA];
}Message;

struct StringPair{
    char username[100];
    char password[100];
};

struct StringPair Login_info[100] = {
    {"Andrew", "password"},
    {"Asim", "test"},
    {"temp", "testing"},
};

bool isValidLogin(const char *username, const char *password) {
    for (int i = 0; i < sizeof(Login_info) / sizeof(Login_info[0]); i++) {
        if (strcmp(Login_info[i].username, username) == 0 &&
            strcmp(Login_info[i].password, password) == 0) {
            return true; // Valid login
        }
    }
    return false; // Invalid login
}

//converting client text into message struct
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
  else if (strncmp (text, "LO_ACK", 6) == 0)
    {
      type = 1;
    }
  else if (strncmp (text, "LO_NAK", 6) == 0)
    {
      type = 2;
      args = 2;
    }
  else if (strncmp (text, "/joinsession", 12) == 0)
    {
      type = 4;
      args = 2;
    }
    else if (strncmp (text, "JN_ACK", 6) == 0)
    {
      type = 5;
      args = 2;
    }
    else if (strncmp (text, "JN_NAK", 6) == 0)
    {
      type = 6;
      args = 3;
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

/*void chat(int connfd){
    char buff[sizeof(Message)]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, sizeof(Message)); 
   
        // read the message from client and copy it in buffer 
        read(connfd, buff, sizeof(buff)); 
        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff); 
        bzero(buff, sizeof(Message)); 
        n = 0; 
        // copy server message in the buffer 
        while ((buff[n++] = getchar()) != '\n') 
            ; 
   
        // and send that buffer to client 
        write(connfd, buff, sizeof(buff)); 
   
        // if msg contains "Exit" then server exit and chat ended. 
        if (strncmp("exit", buff, 4) == 0) { 
            printf("Server Exit\n"); 
            break; 
        } 
    } 
}*/

void chat(int connfd, struct MultiLinkedList* Session_list){
    char buff[sizeof(Message)]; 
    int n; 
    // infinite loop for chat 
    for (;;) { 
        bzero(buff, sizeof(Message)); 
   
        // read the message from client and copy it in buffer 

        // int flags = fcntl(connfd, F_GETFL, 0);
        // fcntl(connfd, F_SETFL, flags | O_NONBLOCK);

        // int bytes_received = 0;
        // int bytes_expected = sizeof(buff);

        // while(bytes_received < bytes_expected){
        //     // ssize_t bytes = read(connfd, buff + bytes_received, bytes_expected - bytes_received);
        //     ssize_t bytes = recv(connfd, buff + bytes_received, bytes_expected - bytes_received, 0);
        //     // if(bytes < 0){
        //     //     if(errno == EAGAIN || errno == EWOULDBLOCK){
        //     //         perror("i dont know what to do here");
        //     //     }
        //     //     else{
        //     //         perror("read");
        //     //     }
        //     // }
        //     // else if(bytes == 0){
        //     //     break;
        //     // }
        //     bytes_received += bytes;            
        // }

        // int total_received = 0;
        // int bytes_received;

        // while ((bytes_received = recv(connfd, buff + total_received, sizeof(buff) - total_received, 0)) > 0) {
        //     total_received += bytes_received;

        //     // Check for end of message (e.g., newline character)
        //     if (buff[total_received - 1] == '\n') {
        //         break;
        //     }
        // }

        read(connfd, buff, 100); 

        // print buffer which contains the client contents 
        printf("From client: %s\t To client : ", buff);

        Message Client_message = stringToMessage(buff);
        //login
        if(Client_message.type == 0){
            //change username and password
            char *inputCopy = strdup(Client_message.data);
            char* UserID = strtok(inputCopy, " ");;
            char* password = strtok(NULL, " ");
             if (isValidLogin(UserID, password)) {
                printf("Login successful\n");
                Message response = textToMessage("LO_ACK", "server"); 
                write(connfd, messageToString(response), sizeof(messageToString(response)));
                
            } else {
                printf("Invalid username or password\n");
                Message response = textToMessage("LO_NAK UserName_or_Password_incorrect", "server"); 
                write(connfd, messageToString(response), sizeof(messageToString(response)));
            }
        }
        //exit
        else if(Client_message.type == 3){
            printf("Server Exit\n"); 
            
            break;
        }
        //join
        else if(Client_message.type == 4){
            if(Join_session(&Session_list, Client_message.data, Client_message.source)){
                printf("Join Session successful\n");
                //write
                size_t combinedSize = strlen("JN_ACK") + strlen(Client_message.data) + 1; // +1 for the space

                // Declare the result array with the exact size
                char result[combinedSize];

                // Use sprintf to combine the strings with a space in between
                sprintf(result, "%s %s", "JN_ACK", Client_message.data);
                
                Message response = textToMessage(result, "server"); 
                write(connfd, messageToString(response), sizeof(messageToString(response)));
                //Sucessfully joined session 
            }
            else{
                printf("Join Session Unsuccessful\n");
                size_t combinedSize = strlen("JN_NAK") + strlen(Client_message.data)+ strlen("Not_a_valid_session") + 1; // +1 for the space

                // Declare the result array with the exact size
                char result[combinedSize];

                // Use sprintf to combine the strings with a space in between
                sprintf(result, "%s %s %s", "JN_NAK", Client_message.data, "Not_a_valid_session");
                
                Message response = textToMessage(result, "server"); 
                write(connfd, messageToString(response), sizeof(messageToString(response)));
            }
        }
        //Leave Session
        else if(Client_message.type == 7){
            if(deletePerson_from_session(&Session_list, "1", "One")){
                //Successfully left session 
                printf("Left Session successful\n");
                Message response = textToMessage("/leavesession", "server"); 
                write(connfd, messageToString(response), sizeof(messageToString(response)));
            }
            else{
                //Did not leave session
                printf("DID NOT Leave Session successful\n");
            }
        }
        //New_session
        else if(Client_message.type == 8){
            create_session(&Session_list, Client_message.data);
            Message response = textToMessage("/createsession", "server"); 
            write(connfd, messageToString(response), sizeof(messageToString(response)));
        }
        //Message
        else if(Client_message.type == 10){
            //
            printf("idk what to do");
            char *Session_name_hes_in = getSessionName(&Session_list, Client_message.source);
            char *names = getNamesInSession(&Session_list, Session_name_hes_in);
            printf("%s", names);


            size_t combinedSize = strlen(names) + strlen(Client_message.data) + 1; // +1 for the space

            // Declare the result array with the exact size
            char result[combinedSize];

            // Use sprintf to combine the strings with a space in between
            sprintf(result, "%s %s", names, Client_message.data);

            Message response = textToMessage(result, Client_message.source); 
            write(connfd, messageToString(response), sizeof(messageToString(response)));
        }
        //Query/List
        else if(Client_message.type == 11){
            char *result = list_of_session(&Session_list);
             // and send that buffer to client 
            write(connfd, result, sizeof(result)); 
            //write list to client and print on there side
        }

        
       
   
        
    } 
}

int main(){
    struct MultiLinkedList Session_list;
    initalize_sessions(&Session_list, 3);
    
    
    

    int sockfd, connfd, len;
    struct sockaddr_in servaddr, cli;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1){
        printf("socket creation failed\n");
        exit(0);
    }
    else{
        printf("socket created successfully\n");
    }
    bzero(&servaddr, sizeof(servaddr));

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification 
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) { 
        printf("socket bind failed\n"); 
        exit(0); 
    } 
    else
        printf("Socket successfully binded\n"); 
   
    // Now server is ready to listen and verification 
    if ((listen(sockfd, 5)) != 0) { 
        printf("Listen failed\n"); 
        exit(0); 
    } 
    else
        printf("Server listening\n"); 
    len = sizeof(cli); 

    // Accept the data packet from client and verification 
    connfd = accept(sockfd, (SA*)&cli, &len); 
    if (connfd < 0) { 
        printf("server accept failed\n"); 
        exit(0); 
    } 
    else{
        printf("server accept the client\n"); 
    }
    // Function for chatting between client and server 
    chat(connfd, &Session_list); 
   
    // After chatting close the socket 
    close(sockfd); 

}
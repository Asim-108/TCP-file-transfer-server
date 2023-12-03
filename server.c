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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

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


void deletePerson_from_session(struct MultiLinkedList* multiList, const char* listName, const char* data) {
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
                    return;
                }

                prev = current;
                current = current->next;
            }

            // Data not found in the list
            printf("Error: Node with data '%s' not found in the list '%s'\n", data, listName);
            return;
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

void Join_session(struct MultiLinkedList* multiList, const char* listName, const char* data) {
    for (int i = 0; i < multiList->numLists; ++i) {
        if (strcmp(multiList->names[i], listName) == 0) {
            // Found the named list, add node to it
            addNode(&multiList->lists[i], data);
            return;
        }
    }
    // If the named list is not found, you can handle the error or provide a message
    printf("Error: List with name '%s' not found\n", listName);
}

// void displayList(struct Node* head) {
//     while (head != NULL) {
//         printf("%s -> ", head->data);
//         head = head->next;
//     }
//     printf("NULL\n");
// }


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

struct Session_struct{
    char Session_number[100];
    char *Usernames[100];
    int num_people;

};

void chat(int connfd){
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
}

int main(){

    struct StringPair Login_info[100] = {
        {"Andrew", "password"},
        {"Asim", "test"},
        {"temp", "testing"},
    };
    int number_sessions = 3;
    struct Session_struct Session[10];
    strcpy(Session[0].Session_number, "0");
    strcpy(Session[1].Session_number, "1");
    strcpy(Session[2].Session_number, "2");

    
    Session[0].num_people = 0;
    Session[1].num_people = 0;
    Session[2].num_people = 0;

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
    chat(connfd); 
   
    // After chatting close the socket 
    close(sockfd); 

}
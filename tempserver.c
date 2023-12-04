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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

struct StringPair{
    char username[100];
    char password[100];
};

//Login Info
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

int main() {
    
    struct MultiLinkedList Session_list;
    initalize_sessions(&Session_list, 3);

    addNode(&Session_list.lists[0], "One");
    addNode(&Session_list.lists[0], "Two");
    addNode(&Session_list.lists[1], "Three");

    

    create_session(&Session_list, "FirstList");


    Join_session(&Session_list, "FirstList", "Four");
    Join_session(&Session_list, "FirstList", "Five");
    deletePerson_from_session(&Session_list, "1", "One");
    
    char *result = list_of_session(&Session_list);
    printf("%s", result);

    char *hell = getSessionName(&Session_list, "Four");
    

    

    char *names = getNamesInSession(&Session_list, hell);
    printf("%s", names);

    free_sessions(&Session_list);

    char test[50];
    strcpy(test, "Asim test");

    char inputCopy[sizeof(test)];
    strcpy(inputCopy, test);
    char* UserID = strtok(inputCopy, " ");;
    char* password = strtok(NULL, " ");

    printf("\n%s %s", UserID, password);
    return 0;
}


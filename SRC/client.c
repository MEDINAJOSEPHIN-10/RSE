/***************************************************************************
*   DATE:
*   AUTHOR : Group-04
*   FILENAME : client.c
*   DESCRIPTION: Contains code for a client that connects to a server 
*               to perform various file operations, including searching for 
*               a file, searching for a string within files, and displaying 
*               file contents. The client communicates with the server via 
*               socket programming and presents options to the user through 
*               a console interface.
*****************************************************************************/

#include "../INC/hdr.h"
#include "../INC/server_function.h"
#include "../INC/loggers.h"
#include "../INC/sf_displayFileContents.h"
#include "../INC/sf_searchForFile.h"
#include "../INC/sf_searchForString.h"
#include <ctype.h> // For isdigit
#include <string.h> // For strlen

// Function to check if a string is a valid path (not purely numeric)
int isValidPath(const char* path) {
    // Check if the path is empty
    if (path == NULL || strlen(path) == 0) {
        return 0; // Invalid path
    }

    int hasNonDigit = 0; // Flag to check for non-digit characters

    // Check each character in the path
    for (size_t i = 0; i < strlen(path); i++) {
        if (!isdigit(path[i]) && path[i] != '/' && path[i] != '.' && path[i] != '_' && path[i] != '-') {
            hasNonDigit = 1; // Found a non-digit character
        }
    }

    // Return 1 (valid path) if there's at least one non-digit character, else return 0 (invalid path)
    return hasNonDigit ? 1 : 0;
}

int main() {
    // Create socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Connect to the server
    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");  // Server IP address
    serverAddr.sin_port = htons(5680);                     // Server port

    if (connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    int choice;
    char buffer[MAX_BUFFER_SIZE];

    while (1) {
        printf("\n");
        printf("***********************************************************\n");
        printf("                   REMOTE SEARCH ENGINE                    \n");
        printf("***********************************************************\n");
        printf("Choose an Option:\n");
        printf("\n");
        printf("1. Search for a file\n");
        printf("2. Search for a string in the filesystem\n");
        printf("3. Enter the absolute path and Display the content\n");
        printf("4. Exit\n");
        printf("************************************************************\n");
        printf("\n");
        printf("Enter your choice: ");
        
        // Input validation for choice
        while (1) {
            if (scanf("%d", &choice) != 1) {
                printf("Invalid input. Please enter a number: ");
                while (getchar() != '\n'); // Clear the input buffer
            } else if (choice < 1 || choice > 4) {
                printf("Please enter a valid option (1-4): ");
            } else {
                break; // Valid input, break out of the loop
            }
        }

        send(clientSocket, &choice, sizeof(choice), 0);

        switch (choice) {
            case 1: {
                char basePath[MAX_BUFFER_SIZE] = "";
                printf("Enter the absolute path: ");
                while (1) {
                    scanf("%s", buffer);
                    // Check if the path is valid
                    if (isValidPath(buffer)) {
                        strcat(basePath, buffer);
                        send(clientSocket, basePath, strlen(basePath) + 1, 0);
                        recv(clientSocket, buffer, sizeof(buffer), 0);
                        printf("Server response:\n%s\n", buffer);
                        break; // Valid path, exit the loop
                    } else {
                        printf("Invalid path. Please enter a correct absolute path (not purely numeric): ");
                    }
                }
                break;
            }
            case 2: {
                printf("Enter the word/sentence/pattern: ");
                getchar();
                scanf("%[^\n]s", buffer);
                send(clientSocket, buffer, strlen(buffer) + 1, 0);
                recv(clientSocket, buffer, sizeof(buffer), 0);
                printf("Server response:\n%s", buffer);
                if (strcmp(buffer, "") == 0) {
                    printf("This string was not found in any file\n\n");
                    break;
                }
                char case2SubChoice[5];
                printf("\n");
                printf("    View a file\n");
                printf("    Enter YES or NO: ");
                scanf("%s", case2SubChoice);
                getchar();
                if (strcmp(case2SubChoice ,  "YES") == 0) {
                    printf("    Enter the path: ");
                    while (1) {
                        scanf("%[^\n]s", buffer);
                        if (isValidPath(buffer)) {
                            send(clientSocket, buffer, strlen(buffer) + 1, 0);
                            recv(clientSocket, buffer, sizeof(buffer), 0);
                            printf("Server response:\n%s\n", buffer);
                            break; // Valid path, exit the loop
                        } else {
                            printf("Invalid path. Please enter a correct absolute path (not purely numeric): ");
                        }
                    }
                } else {
                    send(clientSocket, "", sizeof(buffer), 0);
                }
                break;
            }
            case 3: {
                printf("Enter the absolute path: ");
                getchar();
                while (1) {
                    scanf("%[^\n]s", buffer);
                    if (isValidPath(buffer)) {
                        send(clientSocket, buffer, strlen(buffer) + 1, 0);
                        recv(clientSocket, buffer, sizeof(buffer), 0);
                        printf("Server response:\n%s\n", buffer);
                        break; // Valid path, exit the loop
                    } else {
                        printf("Invalid path. Please enter a correct absolute path (not purely numeric): ");
                    }
                }
                break;
            }
            case 4:
                close(clientSocket);
                exit(EXIT_SUCCESS);

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}


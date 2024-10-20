/***************************************************************************
*   DATE:
*   AUTHOR : Group-04
*	FILENAME : client.c
*	DESCRIPTION: Contains code for a client that connects to a server 
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
        
        scanf("%d", &choice);

        send(clientSocket, &choice, sizeof(choice), 0);

        switch (choice) {
            case 1: ;
              
                char basePath[MAX_BUFFER_SIZE] = "";
                printf("Enter the absolute path: ");
               
                    strcat(basePath, "");
                
                scanf("%s", buffer);
                strcat(basePath, buffer);
                send(clientSocket, basePath, sizeof(buffer), 0);
                recv(clientSocket, buffer, sizeof(buffer), 0);
                printf("Server response:\n%s\n", buffer);
                break;
            case 2:
                printf("Enter the word/sentence/pattern: ");
                getchar();
                scanf("%[^\n]s", buffer);
                send(clientSocket, buffer, sizeof(buffer), 0);
                recv(clientSocket, buffer, sizeof(buffer), 0);
                printf("Server response:\n%s", buffer);
                if (strcmp(buffer, "") == 0) {
                    printf("This string was not found in any file\n\n");
                    break;
                }
                char case2SubChoice[5];
				printf("\n");
                printf("    View a file\n");
                //printf("    2. Do not view file\n");
                printf("    Enter YES or NO: ");

                scanf("%s", case2SubChoice);
                getchar();
                if (strcmp(case2SubChoice ,  "YES") == 0) {
                    printf("    Enter the path: ");
                    scanf("%[^\n]s", buffer);
                    send(clientSocket, buffer, sizeof(buffer), 0);
                    recv(clientSocket, buffer, sizeof(buffer), 0);
                    printf("Server response:\n%s\n", buffer);
                } else {
                    send(clientSocket, "", sizeof(buffer), 0);
                }
                break;
            case 3:
                printf("Enter the absolute path: ");
                getchar();
                scanf("%[^\n]s", buffer);
                send(clientSocket, buffer, sizeof(buffer), 0);
                recv(clientSocket, buffer, sizeof(buffer), 0);
                printf("Server response:\n%s\n", buffer);
                break;
            case 4:
                close(clientSocket);
                exit(EXIT_SUCCESS);

            default:
                printf("Invalid choice.\n");
        }
    }

    return 0;
}

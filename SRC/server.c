#include "../INC/hdr.h"
#include "../INC/server_function.h"
#include "../INC/loggers.h"
#include "../INC/sf_displayFileContents.h"
#include "../INC/sf_searchForFile.h"
#include "../INC/sf_searchForString.h"
#include <pthread.h>

// Mutex for synchronizing file access
pthread_mutex_t fileMutex;

void* handleClient(void* clientSocketPtr) {
    int clientSocket = *((int*)clientSocketPtr);
    free(clientSocketPtr); // Free the allocated memory for clientSocket
    char buffer[MAX_BUFFER_SIZE] = "";
    char result[MAX_BUFFER_SIZE] = "";

    while (1) {
        int choice;
        if (recv(clientSocket, &choice, sizeof(choice), 0) <= 0) {
            LOG_WARN("Client disconnected %s", "");
            break;
        }
        LOG_INFO("Choice of client - %d", choice);

        switch (choice) {
            case 1:
                recv(clientSocket, buffer, sizeof(buffer), 0);
                pthread_mutex_lock(&fileMutex); // Lock the mutex before accessing shared resources
                searchForFile(buffer, result);  // Safely access shared resources
                pthread_mutex_unlock(&fileMutex); // Unlock the mutex after done
                send(clientSocket, result, sizeof(result), 0);
                break;

     case 2:
    // Receive the string to search
    if (recv(clientSocket, buffer, sizeof(buffer), 0) <= 0) {
        LOG_WARN("Failed to receive search string from client %s", "");
        break;
    }
    pthread_mutex_lock(&fileMutex);
    searchForString(buffer, result);  // Search for the string
    pthread_mutex_unlock(&fileMutex);
    
    // Send the result of the search
    send(clientSocket, result, sizeof(result), 0);
    if (strcmp(result, "") == 0) {
        LOG_INFO("No results found for search string %s", buffer);
        break;
    }

    // Receive the path to display
    if (recv(clientSocket, buffer, sizeof(buffer), 0) <= 0) {
        LOG_WARN("Failed to receive file path from client %s", "");
        break;
    }
    if (strcmp(buffer, "") == 0) {
        LOG_WARN("Received empty path from client %s", "");
        break;
    }

    // Display file content
    pthread_mutex_lock(&fileMutex);
    displayFileContent(buffer, result);
    pthread_mutex_unlock(&fileMutex);
    send(clientSocket, result, sizeof(result), 0);
    break;


            case 3:
                recv(clientSocket, buffer, sizeof(buffer), 0);
                pthread_mutex_lock(&fileMutex); // Lock the mutex before accessing shared resources
                displayFileContent(buffer, result);  // Safely access shared resources
                pthread_mutex_unlock(&fileMutex); // Unlock the mutex after done
                send(clientSocket, result, sizeof(result), 0);
                break;

            case 4:
                LOG_INFO("Client exiting %s", "");
                close(clientSocket);
                pthread_exit(NULL); // End the thread
                return NULL;

            default:
                LOG_WARN("Invalid choice from client %s", "");
        }
    }

    close(clientSocket);
    pthread_exit(NULL);
    return NULL;
}

int main() {
    pthread_mutex_init(&fileMutex, NULL); // Initialize the mutex

    LOG_INFO("Creating Socket %s", "");
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        LOG_FATAL("Socket creation failed %s\n", "");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(5680);

    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        LOG_FATAL("Bind failed %s", "");
        exit(EXIT_FAILURE);
    }

    if (listen(serverSocket, 5) == -1) {
        LOG_FATAL("Listen failed %s", "");
        exit(EXIT_FAILURE);
    }

    LOG_INFO("Server listening on port %s...", "5680");

    while (1) {
        int* clientSocketPtr = malloc(sizeof(int)); // Allocate memory for clientSocket
        *clientSocketPtr = accept(serverSocket, NULL, NULL);
        if (*clientSocketPtr == -1) {
            LOG_FATAL("Accept failed %s", "");
            free(clientSocketPtr);
            continue;
        }
        LOG_INFO("Client connected %s", "");

        // Create a new thread for each client
        pthread_t clientThread;
        if (pthread_create(&clientThread, NULL, handleClient, clientSocketPtr) != 0) {
            LOG_FATAL("Failed to create thread %s", "");
            free(clientSocketPtr);
        }

        pthread_detach(clientThread); // Detach thread to handle client independently
    }

    pthread_mutex_destroy(&fileMutex); // Destroy the mutex when done
    close(serverSocket);
    return 0;
} 

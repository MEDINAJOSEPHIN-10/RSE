#ifndef CLIENT_H
#define CLIENT_H

#include <arpa/inet.h> // For struct sockaddr_in and related functions

// Function to create a socket and connect to the server
int connectToServer(int* clientSocket, const char* ipAddress, int port);

// Function to handle user choices based on the user's input
void handleChoice(int clientSocket, int choice);

#endif // CLIENT_H


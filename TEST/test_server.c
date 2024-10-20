#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "../INC/server_function.h"
#include "../INC/sf_searchForFile.h"
#include "../INC/sf_searchForString.h"
#include "../INC/sf_displayFileContents.h"

// Global variables to simulate the active clients count and the mutex
extern int active_clients;
extern pthread_mutex_t client_count_lock;

// Mock functions to simulate client-server interaction
int mock_recv(int socket, void *buffer, size_t length, int flags) {
    static int call_count = 0;
    call_count++;
    
    if (call_count == 1) {
        // Simulate client choice (case 1: Search for file)
        *(int *)buffer = 1;
        return sizeof(int);
    } else if (call_count == 2) {
        // Simulate the search query (search file)
        strcpy(buffer, "test_file.txt");
        return strlen("test_file.txt") + 1;
    } else if (call_count == 3) {
        // Simulate case 4 (client exit)
        *(int *)buffer = 4;
        return sizeof(int);
    }

    return 0;  // Simulate client disconnection
}

int mock_send(int socket, const void *buffer, size_t length, int flags) {
    return length;  // Assume the data was sent successfully
}

void test_handleClient_case1() {
    pthread_mutex_init(&client_count_lock, NULL);
    active_clients = 0;

    // Set up a simulated client socket
    int clientSocket = 1;  // Dummy socket

    // Call the function to test
    handleClient(&clientSocket);

    // Verify the expected behavior
    CU_ASSERT_EQUAL(active_clients, 0);  // Client should have exited
    pthread_mutex_destroy(&client_count_lock);
}

void test_handleClient_case2() {
    pthread_mutex_init(&client_count_lock, NULL);
    active_clients = 0;

    // Set up a simulated client socket
    int clientSocket = 2;  // Dummy socket

    // Call the function to test
    handleClient(&clientSocket);

    // Verify that the client handled a search operation
    CU_ASSERT_EQUAL(active_clients, 0);  // Client should have exited after search
    pthread_mutex_destroy(&client_count_lock);
}

int main() {
    CU_initialize_registry();

    CU_pSuite suite = CU_add_suite("ServerTests", NULL, NULL);

    // Add individual test cases
    CU_add_test(suite, "Test handleClient - Case 1 (Search for File)", test_handleClient_case1);
    CU_add_test(suite, "Test handleClient - Case 2 (Client Exit)", test_handleClient_case2);

    // Run tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();

    CU_cleanup_registry();
    return 0;
}


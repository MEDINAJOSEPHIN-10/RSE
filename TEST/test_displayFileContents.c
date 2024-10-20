#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../INC/hdr.h" // Adjust the path according to your project structure
#include "../INC/loggers.h"

// The function prototype for displayFileContent
void displayFileContent(char* filePath, char* result);

// Test function
void test_displayFileContent(void) {
    // Set up the file and result buffer
    const char* testFilePath = "testfile.txt";
    const char* testContent = "Hello, World!\nThis is a test file.";
    char result[2048] = ""; // Ensure this is large enough to hold expected output

    // Create a temporary file with known content
    FILE* file = fopen(testFilePath, "w");
    CU_ASSERT_PTR_NOT_NULL(file); // Ensure the file was created successfully
    if (file) {
        fprintf(file, "%s", testContent);
        fclose(file);
    }

    // Call the function under test
    displayFileContent((char*)testFilePath, result);

    // Check the result
    CU_ASSERT_STRING_EQUAL(result, "File exists, content of file:-\nHello, World!\nThis is a test file.");

    // Clean up
    remove(testFilePath);
}

// Main function for running the tests
int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Display File Content Test Suite", NULL, NULL);

    CU_add_test(suite, "Test displayFileContent", test_displayFileContent);
    
    CU_basic_run_tests();
    CU_cleanup_registry();
    
    return 0;
}


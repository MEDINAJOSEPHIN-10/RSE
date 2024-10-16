#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server_function.h"
 
// Sample test files
#define TEST_FILE1 "test/file1.txt"
#define TEST_FILE2 "test/file2.txt"
#define TEST_FILE3 "test/file3.txt"
 
// Setup function
void setup(void) {
    FILE *fp1 = fopen(TEST_FILE1, "w");
    FILE *fp2 = fopen(TEST_FILE2, "w");
    FILE *fp3 = fopen(TEST_FILE3, "w");
 
    if (fp1) {
        fprintf(fp1, "This is the first test file.");
        fclose(fp1);
    }
 
    if (fp2) {
        fprintf(fp2, "This file is the second test file.");
        fclose(fp2);
    }
 
    if (fp3) {
        fprintf(fp3, "Third file is here for testing.");
        fclose(fp3);
    }
}
 
// Teardown function
void teardown(void) {
    remove(TEST_FILE1);
    remove(TEST_FILE2);
    remove(TEST_FILE3);
}
 
// Test case for searching a file
void test_searchForFile(void) {
    char result[MAX_BUFFER_SIZE] = "";
 
    // Test for file1.txt
    searchForFile("first test", result);
    CU_ASSERT_STRING_EQUAL(result, "File found: file1.txt\n"); // Adjust based on actual output
 
    // Test for file2.txt
    searchForFile("second test", result);
    CU_ASSERT_STRING_EQUAL(result, "File found: file2.txt\n"); // Adjust based on actual output
 
    // Test for non-existent search
    searchForFile("non-existent string", result);
    CU_ASSERT_STRING_EQUAL(result, "No files found.\n"); // Adjust based on actual output
}
 
int main() {
    CU_initialize_registry();
    CU_pSuite suite = CU_add_suite("Server Function Tests", setup, teardown);
    CU_add_test(suite, "Test Search For File", test_searchForFile);
 
    CU_basic_run_tests();
    CU_cleanup_registry();
 
    return 0;
}

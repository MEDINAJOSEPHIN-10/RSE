#include "../INC/hdr.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../INC/sf_searchForFile.h" // Include the header for the function to be tested
 
// Function prototypes for test cases
void test_searchForFile_fileExists(void);
void test_searchForFile_fileDoesNotExist(void);
void test_searchForFile_emptyResultBuffer(void);
void test_searchForFile_invalidPath(void);
 
// Main function to run tests
int main() {
    // Initialize CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
 
    // Create a test suite within the registry
    CU_pSuite suite = CU_add_suite("FileSearchTests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
 
    // Add test cases to the suite
    if ((CU_add_test(suite, "Test File Exists", test_searchForFile_fileExists) == NULL) ||
        (CU_add_test(suite, "Test File Does Not Exist", test_searchForFile_fileDoesNotExist) == NULL) ||
        (CU_add_test(suite, "Test Empty Result Buffer", test_searchForFile_emptyResultBuffer) == NULL) ||
        (CU_add_test(suite, "Test Invalid Path", test_searchForFile_invalidPath) == NULL)) {
        CU_cleanup_registry();
        return CU_get_error();
    }
 
    // Run the tests using the basic interface
    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
 
    // Clean up and exit
    CU_cleanup_registry();
    return CU_get_error();
}
 
// Test case 1: File exists
void test_searchForFile_fileExists(void) {
    const char* mockFilePath = "existing_file.txt"; // Temporary file path
    const char* mockContent = "Hello, World!";
 
    // Create a temporary file with known content
    FILE* file = fopen(mockFilePath, "w");
    CU_ASSERT_PTR_NOT_NULL(file);  // Ensure the file was created successfully
    if (file != NULL) {
        fprintf(file, "%s", mockContent); // Write the content to the file
        fclose(file);
    } else {
        CU_FAIL("Failed to create test file.");
        return;
    }
 
    // Prepare the result buffer
    char result[2048] = "";
    searchForFile(mockFilePath, result); // Call the function to test
 
    // Check if the result indicates the file exists
    CU_ASSERT_STRING_EQUAL(result, "Yes, the file exists!"); // Expected output
 
    // Clean up: remove the temporary file after testing
    remove(mockFilePath);
}
 
// Test case 2: File does not exist
void test_searchForFile_fileDoesNotExist(void) {
    char result[2048] = "";
    searchForFile("non_existent_file.txt", result); // Call the function with a non-existent file
 
    // Check if the result indicates the file does not exist
    CU_ASSERT_STRING_EQUAL(result, "No, the file does not exist!"); // Expected output
}
 
// Test case 3: Check behavior with empty result buffer (no specific behavior but can ensure it remains intact)
void test_searchForFile_emptyResultBuffer(void) {
    const char* mockFilePath = "empty_result_test_file.txt"; // Temporary file path
 
    // Create a temporary file with no content
    FILE* file = fopen(mockFilePath, "w");
    CU_ASSERT_PTR_NOT_NULL(file);  // Ensure the file was created successfully
    if (file != NULL) {
        fclose(file);
    } else {
        CU_FAIL("Failed to create test file.");
        return;
    }
 
    char result[2048] = ""; // Initialize the result buffer to be empty
    searchForFile(mockFilePath, result); // Call the function to test
 
    // Verify the result remains consistent with no content
    CU_ASSERT_STRING_EQUAL(result, "Yes, the file exists!"); // Expected output for an empty file
 
    // Clean up: remove the temporary file
    remove(mockFilePath);
}
 
// Test case 4: Test invalid path format
void test_searchForFile_invalidPath(void) {
    char result[2048] = "";
    searchForFile("invalid_path_format", result); // Call the function with an invalid path
 
    // Check if the result indicates the file does not exist
    CU_ASSERT_STRING_EQUAL(result, "No, the file does not exist!"); // Expected output
}

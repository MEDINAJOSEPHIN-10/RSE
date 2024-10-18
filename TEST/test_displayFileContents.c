#include "../INC/hdr.h"
#include <CUnit/CUnit.h>
#include <CUnit/Basic.h>
#include "../INC/sf_displayFileContents.h" // Include the header for the function to be tested
 
// Function prototypes for test cases
void test_displayFileContent_fileExists(void);
void test_displayFileContent_fileDoesNotExist(void);
void test_displayFileContent_emptyResultBuffer(void);
 
// Main function to run tests
int main() {
    // Initialize CUnit test registry
    if (CU_initialize_registry() != CUE_SUCCESS) {
        return CU_get_error();
    }
 
    // Create a test suite within the registry
    CU_pSuite suite = CU_add_suite("FileContentDisplayTests", NULL, NULL);
    if (suite == NULL) {
        CU_cleanup_registry();
        return CU_get_error();
    }
 
    // Add test cases to the suite
    if ((CU_add_test(suite, "Test File Exists", test_displayFileContent_fileExists) == NULL) ||
        (CU_add_test(suite, "Test File Does Not Exist", test_displayFileContent_fileDoesNotExist) == NULL) ||
        (CU_add_test(suite, "Test Empty Result Buffer", test_displayFileContent_emptyResultBuffer) == NULL)) {
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
 
// Test case 1: File exists and contains text
void test_displayFileContent_fileExists(void) {
    const char* mockFilePath = "test_file.txt"; // Temporary file path
    const char* mockContent = "Hello, World!\nThis is a test file.\n";
 
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
    displayFileContent(mockFilePath, result); // Call the function to test
 
    // Expected result formatting
    char expectedResult[2048];
    snprintf(expectedResult, sizeof(expectedResult), "File exists, content of file:-\n%s", mockContent);
    CU_ASSERT_STRING_EQUAL(result, expectedResult); // Check if the result matches the expected output
 
    // Clean up: remove the temporary file after testing
    remove(mockFilePath);
}
 
// Test case 2: File does not exist
void test_displayFileContent_fileDoesNotExist(void) {
    char result[2048] = "";
    displayFileContent("non_existent_file.txt", result); // Call the function with a non-existent file
 
    // Check if the result indicates the file does not exist
    CU_ASSERT_STRING_EQUAL(result, "File does not exist\n");
}
 
// Test case 3: Search in an empty temporary file
void test_displayFileContent_emptyResultBuffer(void) {
    const char* mockFilePath = "empty_test_file.txt"; // Temporary file path
 
    // Create an empty temporary file
    FILE* file = fopen(mockFilePath, "w");
    CU_ASSERT_PTR_NOT_NULL(file);  // Ensure the file was created successfully
    if (file != NULL) {
        fclose(file);
    } else {
        CU_FAIL("Failed to create test file.");
        return;
    }
 
    char result[2048] = ""; // Initialize the result buffer to be empty
    displayFileContent(mockFilePath, result); // Call the function to test
 
    // Check if the result contains the expected output for an empty file
    char expectedResult[2048];
    snprintf(expectedResult, sizeof(expectedResult), "File exists, content of file:-\n");
    CU_ASSERT_STRING_EQUAL(result, expectedResult); // Verify the result
 
    // Clean up: remove the temporary file
    remove(mockFilePath);
}

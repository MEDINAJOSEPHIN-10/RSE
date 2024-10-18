/***************************************************************************
*	FILENAME : sf_displayFileContents.c
*	DESCRIPTION:Contains the code to read the contents of a file from the
*	            given file path and store them in a result buffer.
*	            If the file exists, the content is appended to the result;
*	            otherwise, an error message is stored in the buffer.
*	            Logs important events such as file existence and errors.
*****************************************************************************/


#include "../INC/hdr.h"
#include "../INC/loggers.h"

void displayFileContent(char* filePath, char* result) {
    LOG_INFO("Reading contents of file - %s", filePath);
    FILE* file = fopen(filePath, "r");
    if (file != NULL) {
        LOG_INFO("File exists - %s", "TRUE");
        strcat(result, "File exists, content of file:-\n");
        char buffer[1024];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            strcat(result, buffer);
        }
        fclose(file);
    } else {
        strcpy(result, "File does not exist\n");
        LOG_FATAL("File exists - %s", "FALSE");
    }
}

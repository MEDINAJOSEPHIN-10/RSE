#ifndef SF_SEARCH_FOR_STRING_H
#define SF_SEARCH_FOR_STRING_H

void searchForString(const char* searchString, char* result);
void searchInDirectory(const char* dirPath, const char* searchString, char* result);
void searchInFile(const char* filePath, const char* searchString, char* result);

#endif


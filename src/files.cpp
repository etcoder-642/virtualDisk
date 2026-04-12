#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "files.h"

using namespace std;

void virtualFile::createFile(string content, string name, string createdAt, string modifiedAt, long long size, string type, virtualFolder& folder)
{
    virtualFile newFile(content, name, createdAt, modifiedAt, size, type);
    folder.push_back(newFile);
}

void virtualFolder::createFolder(string path, virtualFolder& folder)
{
    virtualFolder newFolder(path);
    folder.push_back(newFolder);
}
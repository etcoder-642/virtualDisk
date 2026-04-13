#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#include "files.h"

using namespace std;

shared_ptr<virtualFile> virtualFolder::createFile(string content, string name, string type)
{
    auto fileptr = make_shared<virtualFile>(content, name, this, type);
    this->addEntity(fileptr);
    return fileptr;
}

void virtualFolder::addEntity(shared_ptr<FileSystemEntity> entity) {
    contents.push_back(entity);
    memberCount++;
}

shared_ptr<virtualFolder> virtualFolder::createFolder(string name)
{
    auto folderptr = make_shared<virtualFolder>(name, this);
    this->addEntity(folderptr);
    return folderptr;
}
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "files.h"

using namespace std;

shared_ptr<virtualFile> virtualFolder::createFile(string content, string name, string type)
{
    auto fileptr = make_shared<virtualFile>(content, name, this, type);
    this->addEntity(fileptr);
    return fileptr;
}

void virtualFolder::addEntity(shared_ptr<FileSystemEntity> entity)
{
    contents.push_back(entity);
    memberCount++;
}

shared_ptr<virtualFolder> virtualFolder::createFolder(string name)
{
    auto folderptr = make_shared<virtualFolder>(name, this);
    this->addEntity(folderptr);
    return folderptr;
}

vector<string> virtualFolder::buildAncestorsList(FileSystemEntity *initialNode)
{
    vector<string> res;
    FileSystemEntity* node = initialNode;
    while (node != nullptr)
    {
        res.push_back(node->getName());
        node = node->getParentNode();
    }
    reverse(res.begin(), res.end());
    return res;
}

bool virtualFolder::checkFolderExistence(string folderName)
{
    for(int i = 0; i < contents.size(); i++)
    {
        if(contents[i]->isFolder())
        {
            if(contents[i]->getName() == folderName)
            {
                return true;
            }
        }
    }
    return false;
}

shared_ptr<FileSystemEntity> virtualFolder::getPointerFromName(string name)
{
    for(const auto& entity: contents)
    {
        if(entity->getName() == name)
        {
            return entity;
        }
    }
    return nullptr;
}
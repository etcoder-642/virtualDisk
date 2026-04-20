#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "files.h"
#include "utils.h"

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
    for(size_t i = 0; i < contents.size(); i++)
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

virtualFolder* FileSystem::changeDirectory(string path)
{
    vector<string> actionList = parseInputs(path, '/');
    for (size_t i = 0; i < actionList.size(); i++)
    {
        if (actionList[i] == "..")
        {
            if (cwd->getParentNode() == nullptr)
            {
                cout << "You're on the root folder!!!";
                break;
            }
            cwd = static_cast<virtualFolder *>(cwd->getParentNode());
        }
        else if (actionList[i] == ".")
        {
            continue;
        }
        else if (actionList[i] == "~")
        {
            cwd = root.get();
        }
        else
        {
            if (cwd->checkFolderExistence(actionList[i]))
            {
                cwd = static_cast<virtualFolder *>(cwd->getPointerFromName(actionList[i]).get());
            }
        }
    }
    return cwd;
}

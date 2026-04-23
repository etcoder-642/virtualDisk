#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "files.h"
#include "utils.h"
#include "display.h"
#include "validation.h"

using namespace std;

vector<string> virtualFile::validTypes = {
    "txt", "json", "xml", "dat", "cfg", "save",
    "png", "jpg", "wav", "ogg", "mp3", "mp4",
    "lvl", "map", "log", "tmp", "pdf", "zip"
};

void virtualFile::registerFileType(string str)
{
    if (find(validTypes.begin(), validTypes.end(), str) == validTypes.end())
    {
        validTypes.push_back(str);
    }
}

bool virtualFile::checkFileTypeExistence(string str)
{
    for(size_t i = 0;i < validTypes.size(); i++){
        if(str == validTypes[i]) return true;
    }
    return false;
}
shared_ptr<virtualFile> virtualFolder::createFile(string content, string name, string type, Validator INPUT_VALIDATOR)
{
    vector<string> files = this->getFilesName();
    if(find(files.begin(), files.end(), name) != files.end())
    {
        INPUT_VALIDATOR.setErrorMessage("Error: A file with the same name already exists in this directory.");
        INPUT_VALIDATOR.setSuggestion("Use another name.");
        return nullptr;
    }
    auto fileptr = make_shared<virtualFile>(content, name, this, type);
    this->addEntity(fileptr);
    return fileptr;
}

void virtualFolder::addEntity(shared_ptr<FileSystemEntity> entity)
{
    contents.push_back(entity);
    memberCount++;
}

shared_ptr<virtualFolder> virtualFolder::createFolder(string name, Validator INPUT_VALIDATOR)
{
    vector<string> folders = this->getFoldersName();
    if(find(folders.begin(), folders.end(), name) != folders.end())    
    {
        INPUT_VALIDATOR.setErrorMessage("Error: A folder with the same name already exists in this directory.");
        INPUT_VALIDATOR.setSuggestion("Use another name.");
        return nullptr;
    }
    auto folderptr = make_shared<virtualFolder>(name, this);
    this->addEntity(folderptr);
    return folderptr;
}

vector<string> virtualFolder::getFoldersName() const
{
    vector<string> res;
    for (const auto &s : contents)
    {
        if (s->isFolder())
        {
            res.push_back(s->getName());
        }
    }
    return res;
}

vector<string> virtualFolder::getFilesName() const
{
    vector<string> res;
    for (const auto &s : contents)
    {
        if (!(s->isFolder()))
        {
            res.push_back(s->getName());
        }
    }
    return res;
}

vector<string> virtualFolder::buildAncestorsList(FileSystemEntity *initialNode)
{
    vector<string> res;
    FileSystemEntity *node = initialNode;
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
    for (size_t i = 0; i < contents.size(); i++)
    {
        if (contents[i]->isFolder())
        {
            if (contents[i]->getName() == folderName)
            {
                return true;
            }
        }
    }
    return false;
}

shared_ptr<FileSystemEntity> virtualFolder::getPointerFromName(string name)
{
    for (const auto &entity : contents)
    {
        if (entity->getName() == name)
        {
            return entity;
        }
    }
    return nullptr;
}

virtualFolder *FileSystem::changeDirectory(string path)
{
    vector<string> actionList = parseInputs(path, '/');
    for (size_t i = 0; i < actionList.size(); i++)
    {
        if (actionList[i] == "..")
        {
            if (cwd->getParentNode() == nullptr)
            {
                displaySpecialMessage("You're on the root folder!!!");
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
            }else{
                displaySpecialMessage("Error: No such directory exists.");
            }
        }
    }
    return cwd;
}

virtualFolder* FileSystem::traverseTree(string path)
{
    vector<string> actionList = parseInputs(path, '/');
    virtualFolder* node = cwd;
    for (size_t i = 0; i < actionList.size(); i++)
    {
        if (actionList[i] == "..")
        {
            if (node->getParentNode() == nullptr)
            {
                displaySpecialMessage("You're on the root folder!!!");
                break;
            }
            node = static_cast<virtualFolder *>(node->getParentNode());
        }
        else if (actionList[i] == ".")
        {
            continue;
        }
        else if (actionList[i] == "~")
        {
            node = root.get();
        }
        else
        {
            if (node->checkFolderExistence(actionList[i]))
            {
                node = static_cast<virtualFolder *>(node->getPointerFromName(actionList[i]).get());
            }else{
                displaySpecialMessage("Error: No such directory exists.");
            }
        }
    }
    return node;

}

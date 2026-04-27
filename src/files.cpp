#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "../include/files.h"
#include "../include/utils.h"
#include "../include/display.h"
#include "../include/validation.h"

using namespace std;

auto FileSystemEntity::getParentAsFolder() const
{
    return dynamic_pointer_cast<virtualFolder>(parent.lock());
}

vector<string> virtualFile::validTypes = {
    "txt", "json", "xml", "dat", "cfg", "save",
    "png", "jpg", "wav", "ogg", "mp3", "mp4",
    "lvl", "map", "log", "tmp", "pdf", "zip"};

void virtualFile::registerFileType(string str)
{
    if (find(validTypes.begin(), validTypes.end(), str) == validTypes.end())
    {
        validTypes.push_back(str);
    }
}

bool virtualFile::checkFileTypeExistence(string str)
{
    for (size_t i = 0; i < validTypes.size(); i++)
    {
        if (str == validTypes[i])
            return true;
    }
    return false;
}
shared_ptr<virtualFile> virtualFolder::createFile(string content, string name, string type, Validator &INPUT_VALIDATOR)
{
    auto self = static_pointer_cast<virtualFolder>(shared_from_this());
    vector<string> files = self->getFilesName();
    if (find(files.begin(), files.end(), name) != files.end())
    {
        INPUT_VALIDATOR.setErrorMessage("Error: A file with the same name already exists in this directory.");
        INPUT_VALIDATOR.setSuggestion("Use another name.");
        return nullptr;
    }
    auto fileptr = make_shared<virtualFile>(content, name, self, type);
    self->addEntity(fileptr);
    return fileptr;
}

void virtualFolder::addEntity(shared_ptr<FileSystemEntity> entity)
{
    contents.push_back(entity);
    memberCount++;
}

shared_ptr<virtualFolder> virtualFolder::createFolder(string name, Validator &INPUT_VALIDATOR)
{
    auto self = static_pointer_cast<virtualFolder>(shared_from_this());
    vector<string> folders = self->getFoldersName();
    if (find(folders.begin(), folders.end(), name) != folders.end())
    {
        INPUT_VALIDATOR.setErrorMessage("Error: A folder with the same name already exists in this directory.");
        INPUT_VALIDATOR.setSuggestion("Use another name.");
        return nullptr;
    }
    auto folderptr = make_shared<virtualFolder>(name, self);
    self->addEntity(folderptr);
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

vector<string> virtualFolder::buildAncestorsList(weak_ptr<FileSystemEntity> initialNode)
{
    vector<string> res;
    auto node = initialNode.lock();
    while (node != nullptr)
    {
        res.push_back(node->getName());
        node = node->getParentNode().lock();
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

shared_ptr<virtualFolder> virtualFolder::getPointerFromNameAsFolder(string name)
{
    for (const auto &entity : contents)
    {
        if (entity->getName() == name)
        {
            return dynamic_pointer_cast<virtualFolder>(entity);
        }
    }
    return nullptr;
}

weak_ptr<virtualFolder> FileSystem::traverseTree(string path, Validator &INPUT_VALIDATOR)
{
    vector<string> actionList = parseInputs(path, '/');
    weak_ptr<virtualFolder> node = cwd;
    for (size_t i = 0; i < actionList.size(); i++)
    {
        auto currentShared = node.lock();
        if (!currentShared)
        {
            INPUT_VALIDATOR.setErrorMessage("Error: Current directory is invalid.");
            INPUT_VALIDATOR.setSuggestion("Please check the current directory.");
            return weak_ptr<virtualFolder>();
        }
        if (actionList[i] == "..")
        {
            auto parentFolder = currentShared->getParentAsFolder();
            if (parentFolder)
            {
                node = parentFolder;
            }
            else
            {
                INPUT_VALIDATOR.setErrorMessage("Error: Already at root directory.");
                INPUT_VALIDATOR.setSuggestion("Cannot go up from root directory.");
                return weak_ptr<virtualFolder>();
            }
        }
        else if (actionList[i] == ".")
        {
            continue;
        }
        else if (actionList[i] == "~")
        {
            node = root;
        }
        else
        {
            if (currentShared->checkFolderExistence(actionList[i]))
            {
                auto nextFolder = currentShared->getPointerFromNameAsFolder(actionList[i]);
                if (!nextFolder)
                {
                    INPUT_VALIDATOR.setErrorMessage("Error: " + actionList[i] + " is not a directory.");
                    INPUT_VALIDATOR.setSuggestion("Please check the directory path.");
                    return weak_ptr<virtualFolder>();
                }
                else
                {
                    node = nextFolder;
                }
            }
            else
            {
                INPUT_VALIDATOR.setErrorMessage("Error: No such directory exists.");
                INPUT_VALIDATOR.setSuggestion("Please check the directory path.");
                return weak_ptr<virtualFolder>();
            }
        }
    }
    return node;
}

shared_ptr<virtualFolder> FileSystem::traverseTree_S(string path, Validator &INPUT_VALIDATOR)
{
    auto node = traverseTree(path, INPUT_VALIDATOR).lock();
    if (!node)
    {
        INPUT_VALIDATOR.setErrorMessage("Error: Directory does not exist.");
        INPUT_VALIDATOR.setSuggestion("Please check the directory path.");
    }
    return node;
}

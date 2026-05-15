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
#include "../include/error/error.h"
#include "../include/error/result.h"

using namespace std;

namespace vfs
{
    // FILE SYSTEM ENTITY METHODS
    auto FileSystemEntity::getParentAsFolder() const
    {
        return dynamic_pointer_cast<virtualFolder>(parent.lock());
    }

    // VIRTUAL FILE METHODS

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

    // VIRTUAL FOLDER METHODS
    vector<string> virtualFolder::getContentNames() const
    {
        vector<string> res;
        for (const auto &s : contents)
        {
            res.push_back(s->getName());
        }
        return res;
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

    // Modify Methods
    void virtualFolder::addEntity(shared_ptr<FileSystemEntity> entity)
    {
        contents.push_back(entity);
        memberCount++;
    }

    Result<shared_ptr<virtualFile>> virtualFolder::createFile(string content, string name, string type)
    {
        auto self = static_pointer_cast<virtualFolder>(shared_from_this());
        vector<string> files = self->getFilesName();
        if (find(files.begin(), files.end(), name) != files.end())
        {
            return Result<shared_ptr<virtualFile>>::Err(
                "Error: A file with the same name already exists in this directory.",
                "Use another name.");
        }
        auto fileptr = make_shared<virtualFile>(content, name, self, type);
        self->addEntity(fileptr);
        return Result<shared_ptr<virtualFile>>::Ok(fileptr);
    }

    Result<shared_ptr<virtualFolder>> virtualFolder::createFolder(string name)
    {
        auto self = static_pointer_cast<virtualFolder>(shared_from_this());
        vector<string> folders = self->getFoldersName();
        if (find(folders.begin(), folders.end(), name) != folders.end())
        {
            return Result<shared_ptr<virtualFolder>>::Err(
                "Error: A folder with the same name already exists in this directory.",
                "Use another name.");
        }
        auto folderptr = make_shared<virtualFolder>(name, self);
        self->addEntity(folderptr);
        return Result<shared_ptr<virtualFolder>>::Ok(folderptr);
    }

    Result<void> virtualFolder::removeFile(shared_ptr<virtualFile> file)
    {
        if (!file)
            return Result<void>::Err(
                "Error: Null File Provided.",
                "Please check the file name.");
        auto it = remove(contents.begin(), contents.end(), file);
        if (it != contents.end())
        {
            contents.erase(it, contents.end());
            memberCount--;
            return Result<void>::Ok();
        }

        return Result<void>::Err(
            "Error: No such file exists in this directory.",
            "Please check the file name.");
    }

    Result<shared_ptr<virtualFolder>> virtualFolder::removeFolder(shared_ptr<virtualFolder> folder, shared_ptr<virtualFolder> cwd)
    {
        if (!folder)
            return Result<shared_ptr<virtualFolder>>::Err(
                "Error: Null Folder Provided.",
                "Please check the folder name.");

        if (cwd == folder)
        {
            cwd = folder->getParentAsFolder();
        }

        auto it = remove(contents.begin(), contents.end(), folder);
        if (it == contents.end())
        {
            return Result<shared_ptr<virtualFolder>>::Err(
                "Error: No such folder exists in this directory.",
                "Please check the folder name.");
        }
        contents.erase(it, contents.end());
        memberCount--;
        return Result<shared_ptr<virtualFolder>>::Ok(cwd);
    }

    // Query Methods

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

    Result<shared_ptr<virtualFolder>> virtualFolder::getPointerFromNameAsFolder(string &name)
    {
        for (const auto &entity : contents)
        {
            if (entity->getName() == name)
            {
                auto folderptr = dynamic_pointer_cast<virtualFolder>(entity);
                if (!folderptr)
                {
                    return Result<shared_ptr<virtualFolder>>::Err(
                        "Error: '" + name + "' is a file, not a folder.",
                        "Try using file-specific commands or check the name.");
                }
                return Result<shared_ptr<virtualFolder>>::Ok(folderptr);
            }
        }
        return Result<shared_ptr<virtualFolder>>::Err(
            "Error: Folder not found.",
            "Please check the folder name.");
    }

    Result<shared_ptr<virtualFile>> virtualFolder::getPointerFromNameAsFile(string &name)
    {
        for (const auto &entity : contents)
        {
            if (entity->getName() == name)
            {
                auto fileptr = dynamic_pointer_cast<virtualFile>(entity);
                if (!fileptr)
                {
                    return Result<shared_ptr<virtualFile>>::Err(
                        "Error: '" + name + "' is a directory, not a file.",
                        "Try using directory-specific commands or check the name.");
                }
                return Result<shared_ptr<virtualFile>>::Ok(fileptr);
            }
        }
        return Result<shared_ptr<virtualFile>>::Err(
            "Error: File not found.",
            "Please check the file name.");
    }

    //  FILE SYSTEM CLASS METHODS

    Result<weak_ptr<virtualFolder>> FileSystem::traverseTree(string &path)
    {
        vector<string> actionList = parseInputs(path, '/');
        weak_ptr<virtualFolder> node = cwd;
        for (size_t i = 0; i < actionList.size(); i++)
        {
            auto currentShared = node.lock();
            if (!currentShared)
            {
                return Result<weak_ptr<virtualFolder>>::Err(
                    "Error: Current directory is invalid.",
                    "Please check the current directory.");
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
                    return Result<weak_ptr<virtualFolder>>::Err(
                        "Error: Already at root directory.",
                        "Cannot go up from root directory.");
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
                    if (nextFolder.isErr())
                    {
                        return Result<weak_ptr<virtualFolder>>::Err(
                            "Error: " + actionList[i] + " is not a directory.",
                            "Please check the directory path.");
                    }
                    else
                    {
                        node = nextFolder.unwrap();
                    }
                }
                else
                {
                    return Result<weak_ptr<virtualFolder>>::Err(
                        "Error: No such directory exists.",
                        "Please check the directory path.");
                }
            }
        }
        return Result<weak_ptr<virtualFolder>>::Ok(node);
    }

    Result<shared_ptr<virtualFolder>> FileSystem::traverseTree_S(string &path)
    {
        auto node = traverseTree(path).unwrap().lock();
        if (!node)
        {
            return Result<shared_ptr<virtualFolder>>::Err(
                "Error: Directory does not exist.",
                "Please check the directory path.");
        }
        return Result<shared_ptr<virtualFolder>>::Ok(node);
    }
}

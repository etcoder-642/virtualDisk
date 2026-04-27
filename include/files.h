#ifndef FILES_H
#define FILES_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <ctime>
#include <chrono>
#include <format>

#include "validation.h"

using namespace std;

class virtualFolder;
class FileSystemEntity : public enable_shared_from_this<FileSystemEntity>
{
protected:
    string name;
    string createdAt;
    string modifiedAt;
    weak_ptr<FileSystemEntity> parent;

public:
    FileSystemEntity(string n, weak_ptr<FileSystemEntity> p)
        : name(n), parent(p)
    {
        auto now = std::time(nullptr);
        auto *now_tm = std::localtime(&now);

        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", now_tm);
        createdAt = std::string(buffer);
        modifiedAt = createdAt;
    }
    virtual ~FileSystemEntity() = default;
    string getCreatedAt() const { return createdAt; }
    string getModifiedAt() const { return modifiedAt; }
    auto getParentNode() const { return parent; }
    auto getParentAsFolder() const;
    string getName() const { return name; }

    void setName(string newName) { name = newName; }
    void setCreatedAt(string newCreatedAt) { createdAt = newCreatedAt; }
    void setModifiedAt(string newModifiedAt) { modifiedAt = newModifiedAt; }
    
    virtual bool isFolder() const = 0;
};

class virtualFile : public FileSystemEntity
{
private:
    string content;
    string type;
    long long size;

    static vector<string> validTypes;

public:
    virtualFile(string content, string name, weak_ptr<FileSystemEntity> parent, string type = "txt")
        : FileSystemEntity(name, parent), content(content), type(type), size(content.size()) {}

    string getContent() const { return content; }
    long long getSize() const { return size; }
    string getType() const { return type; }

    bool isFolder() const override { return false; }

    void setContent(string newContent)
    {
        content = newContent;
        size = content.size();
    }

    void setType(string newType)
    {
        type = newType;
    }
    static void registerFileType(string str);
    static bool checkFileTypeExistence(string str);
};

class virtualFolder : public FileSystemEntity
{
private:
    long long totalSize;
    int memberCount;
    vector<shared_ptr<FileSystemEntity>> contents;

public:
    virtualFolder(string name, weak_ptr<FileSystemEntity> parent)
        : FileSystemEntity(name, parent), totalSize(0), memberCount(0) {}

    long long getTotalSize() const { return totalSize; }
    int getMemberCount() const { return memberCount; }
    bool isFolder() const override { return true; }

    vector<shared_ptr<FileSystemEntity>> getContents() const { return contents; }
    vector<string> getContentNames() const
    {
        vector<string> res;
        for (const auto &s : contents)
        {
            res.push_back(s->getName());
        }
        return res;
    }

    vector<string> getFoldersName() const;
    vector<string> getFilesName() const;
    shared_ptr<virtualFile> createFile(string content, string name, string type, Validator& INPUT_VALIDATOR);
    void addEntity(shared_ptr<FileSystemEntity> entity);
    shared_ptr<virtualFolder> createFolder(string name, Validator& INPUT_VALIDATOR);
    vector<string> buildAncestorsList(weak_ptr<FileSystemEntity> initialNode);
    bool checkFolderExistence(string folderName);
    shared_ptr<FileSystemEntity> getPointerFromName(string name);
    shared_ptr<virtualFolder> getPointerFromNameAsFolder(string name);
};


class FileSystem {
private:
    shared_ptr<virtualFolder> root;
    weak_ptr<virtualFolder> cwd;
public:
    FileSystem() {
        root = make_shared<virtualFolder>("root", weak_ptr<FileSystemEntity>());
        cwd = root;
    }
    shared_ptr<virtualFolder> getRoot() const { return root; }
    weak_ptr<virtualFolder> getCWD() const { return cwd; }

    void setCWD(weak_ptr<virtualFolder> newCWD){
        cwd = newCWD;
    }

    shared_ptr<virtualFolder> getCWD_S() const { // getCWD_S stands for getCWD_Shared, it returns a shared_ptr instead of weak_ptr
        return cwd.lock();
    }
    weak_ptr<virtualFolder> traverseTree(string path, Validator& INPUT_VALIDATOR);
    shared_ptr<virtualFolder> traverseTree_S(string path, Validator& INPUT_VALIDATOR);
};

#endif
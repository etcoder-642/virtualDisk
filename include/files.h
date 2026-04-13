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

using namespace std;

class FileSystemEntity
{
protected:
    string name;
    string createdAt;
    string modifiedAt;
    FileSystemEntity *parent;

public:
    FileSystemEntity(string n, FileSystemEntity *p)
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

    void setName(string newName) { name = newName; }
    void setCreatedAt(string newCreatedAt) { createdAt = newCreatedAt; }
    void setModifiedAt(string newModifiedAt) { modifiedAt = newModifiedAt; }
    virtual string getName() = 0;
};

class virtualFile : public FileSystemEntity
{
private:
    string content;
    string type;
    long long size;

public:
    virtualFile(string content, string name, FileSystemEntity *parent, string type = "txt")
        : content(content), FileSystemEntity(name, parent), type(type), size(content.size()) {}

    string getName() override { return name; }
    string getContent() const { return content; }
    long long getSize() const { return size; }
    string getType() const { return type; }

    void setContent(string newContent)
    {
        content = newContent;
        size = content.size();
    }

    void setType(string newType)
    {
        type = newType;
    }
};

class virtualFolder : public FileSystemEntity
{
private:
    long long totalSize;
    int memberCount;
    vector<shared_ptr<FileSystemEntity>> contents;

public:
    virtualFolder(string name, FileSystemEntity *parent)
        : FileSystemEntity(name, parent), totalSize(0), memberCount(0) {}

    string getName() override { return name; }
    long long getTotalSize() const { return totalSize; }
    int getMemberCount() const { return memberCount; }
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

    shared_ptr<virtualFile> createFile(string content, string name, string type);
    void addEntity(shared_ptr<FileSystemEntity> entity);
    shared_ptr<virtualFolder> createFolder(string name);
};

#endif
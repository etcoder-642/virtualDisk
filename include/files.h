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


class FileSystemEntity {
protected:
    string name;
    string createdAt;
    string modifiedAt;
    FileSystemEntity* parent;
public:
    FileSystemEntity(string n, FileSystemEntity* p)
        : name(n), parent(p)  {
            auto now = chrono::system_clock::now();
            auto now_c = chrono::system_clock::to_time_t(now);
            createdAt = format("{:%Y-%m-%d %H:%M:%S}", *localtime(&now_c));
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
    virtualFile(string name, FileSystemEntity* parent, string type = "txt")
        : content(""), FileSystemEntity(name, parent), type(type), size(0) {}
    string getName() override { return name; }
    long long getSize() const { return size; }
    string getType() const { return type; }
    
    void setSize(long long newSize) {
        size = newSize;
    }

    void setType(string newType) {
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
    virtualFolder(string name, FileSystemEntity* parent)
        : FileSystemEntity(name, parent), totalSize(0), memberCount(0) {}
};

#endif
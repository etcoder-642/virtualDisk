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
#include "error/error.h"
#include "error/result.h"

namespace vfs
{
    class virtualFolder;
    class FileSystemEntity : public std::enable_shared_from_this<FileSystemEntity>
    {
    protected:
        std::string name;
        std::string createdAt;
        std::string modifiedAt;
        std::weak_ptr<FileSystemEntity> parent;

    public:
        FileSystemEntity(std::string n, std::weak_ptr<FileSystemEntity> p)
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
        std::string getCreatedAt() const { return createdAt; }
        std::string getModifiedAt() const { return modifiedAt; }
        auto getParentNode() const { return parent; }
        auto getParentAsFolder() const;
        std::string getName() const { return name; }

        void setName(std::string newName) { name = std::move(newName); }
        void setModifiedAt(std::string newModifiedAt) { modifiedAt = std::move(newModifiedAt); }

        virtual bool isFolder() const = 0;
    };

    class virtualFile : public FileSystemEntity
    {
    private:
        std::string content;
        std::string type;
        long long size;

        static std::vector<std::string> validTypes;

    public:
        virtualFile(std::string content, std::string name, std::weak_ptr<FileSystemEntity> parent, std::string type = "txt")
            : FileSystemEntity(name, parent), content(content), type(type), size(content.size()) {}

        std::string getContent() const { return content; }
        long long getSize() const { return size; }
        std::string getType() const { return type; }

        bool isFolder() const override { return false; }

        void setContent(std::string newContent)
        {
            content = std::move(newContent);
            size = content.size();
        }

        void setType(std::string newType)
        {
            type = newType;
        }
        static void registerFileType(std::string str);
        static bool checkFileTypeExistence(std::string str);
    };

    class virtualFolder : public FileSystemEntity
    {
    private:
        long long totalSize;
        int memberCount;
        std::vector<std::shared_ptr<FileSystemEntity>> contents;

    public:
        virtualFolder(std::string name, std::weak_ptr<FileSystemEntity> parent)
            : FileSystemEntity(name, parent), totalSize(0), memberCount(0) {}

        // Getters and Setters
        long long getTotalSize() const { return totalSize; }
        int getMemberCount() const { return memberCount; }
        bool isFolder() const override { return true; }

        std::vector<std::shared_ptr<FileSystemEntity>> getContents() const { return contents; }
        std::vector<std::string> getContentNames() const;
        std::vector<std::string> getFoldersName() const;
        std::vector<std::string> getFilesName() const;

        // Modify Methods
        void addEntity(std::shared_ptr<FileSystemEntity> entity);
        Result<std::shared_ptr<virtualFile>> createFile(std::string content, std::string name, std::string type);
        Result<std::shared_ptr<virtualFolder>> createFolder(std::string name);
        Result<void> removeFile(std::shared_ptr<virtualFile> file);
        Result<void> removeFileByName(std::string name);
        Result<std::shared_ptr<virtualFolder>> removeFolder(std::shared_ptr<virtualFolder> folder, std::shared_ptr<virtualFolder> cwd);

        // Query Methods
        bool checkFolderExistence(std::string folderName);
        std::vector<std::string> buildAncestorsList(std::weak_ptr<FileSystemEntity> initialNode);
        std::shared_ptr<FileSystemEntity> getPointerFromName(std::string name);
        Result<std::shared_ptr<virtualFolder>> getPointerFromNameAsFolder(std::string &name);
        Result<std::shared_ptr<virtualFile>> getPointerFromNameAsFile(std::string &name);
    };

    class FileSystem
    {
    private:
        std::shared_ptr<virtualFolder> root;
        std::weak_ptr<virtualFolder> cwd;

    public:
        FileSystem()
        {
            root = std::make_shared<virtualFolder>("root", std::weak_ptr<FileSystemEntity>());
            cwd = root;
        }
        std::shared_ptr<virtualFolder> getRoot() const { return root; }
        std::weak_ptr<virtualFolder> getCWD() const { return cwd; }

        void setCWD(std::weak_ptr<virtualFolder> newCWD)
        {
            cwd = newCWD;
        }

        // getCWD_S stands for getCWD_Shared, it returns a shared_ptr instead of weak_ptr
        std::shared_ptr<virtualFolder> getCWD_S() const
        { 
            return cwd.lock();
        }
        Result<std::weak_ptr<virtualFolder>> traverseTree(std::string &path);
        Result<std::shared_ptr<virtualFolder>> traverseTree_S(std::string &path);
    };

}

#endif
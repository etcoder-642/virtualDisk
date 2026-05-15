#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "../include/files.h"
#include "../include/display.h"
#include "../include/utils.h"
#include "../include/validation.h"
#include "../include/error/error.h"
#include "../include/error/result.h"


namespace vfs
{
    enum class CommandCode
    {
        MKDIR,
        LS,
        CD,
        TOUCH,
        RM,
        CAT,
        HELP,
        EXIT,
        UNKNOWN
    };

}

using namespace vfs;
using namespace std;

int main()
{
    FileSystem fs;

    map<string, CommandCode> commandMap = {
        {"mkdir", CommandCode::MKDIR},
        {"ls", CommandCode::LS},
        {"cd", CommandCode::CD},
        {"touch", CommandCode::TOUCH},
        {"rm", CommandCode::RM},
        {"cat", CommandCode::CAT},
        {"help", CommandCode::HELP},
        {"exit", CommandCode::EXIT},

        {"create-directory", CommandCode::MKDIR},
        {"list", CommandCode::LS},
        {"change-directory", CommandCode::CD},
        {"create-file", CommandCode::TOUCH},
        {"remove", CommandCode::RM},
        {"concatenate", CommandCode::CAT}};
    CommandCode code = CommandCode::UNKNOWN;
    initialPage();

    string userChoice;

    const auto cur_path_ = fs.getCWD_S()->buildAncestorsList(fs.getCWD());
    displayCurrentPath(cur_path_);
    while (code != CommandCode::EXIT && getline(cin, userChoice))
    {
        vector<string> parts = parseInputs(userChoice, ' ');
        auto syntaxCheck = Validator::syntaxCheckerInput(parts);
        if (syntaxCheck.isErr())
        {
            displayError(syntaxCheck.unwrapErr().message, syntaxCheck.unwrapErr().suggestion);
            displayCurrentPath(cur_path_);
            continue;
        }

        string cmd = parts[0];

        vector<string> args(parts.begin() + 1, parts.end());

        code = commandMap.count(cmd) ? commandMap[cmd] : CommandCode::UNKNOWN;

        switch (code)
        {
        case CommandCode::MKDIR:
        {
            auto syntaxCheck_mkdir_ = Validator::syntaxCheckerMKDIR(args);
            if (syntaxCheck_mkdir_.isErr())
            {
                displayError(syntaxCheck_mkdir_.unwrapErr().message, syntaxCheck_mkdir_.unwrapErr().suggestion);
                break;
            }
            for (string str : args)
            {
                vector<string> parts = parseInputs(str, '/');
                vector<string> destinationPath(parts.begin(), parts.end() - 1);
                string folderName = parts.back();

                string input = destinationPath.empty() ? "." : joinStrings(destinationPath, '/');

                auto traverseTree_ = fs.traverseTree_S(input);
                if (traverseTree_.isErr())
                {
                    displayError(traverseTree_.unwrapErr().message, traverseTree_.unwrapErr().suggestion);
                    continue;
                }

                shared_ptr<virtualFolder> destination = traverseTree_.unwrap();

                auto isValidName_ = Validator::isValidName(folderName);
                if (isValidName_.isErr())
                {
                    displayError(isValidName_.unwrapErr().message, isValidName_.unwrapErr().suggestion);
                    continue;
                }

                auto createdFolder_ = destination->createFolder(folderName);
                if (createdFolder_.isErr())
                {
                    displayError(
                        createdFolder_.unwrapErr().message,
                        createdFolder_.unwrapErr().suggestion);
                }
                else
                {
                    displaySpecialMessage(folderName + " Folder Created successfully!!!");
                }
            }
        }
        break;
        case CommandCode::LS:
        {
            shared_ptr<virtualFolder> destination;
            map<string, vector<string>> multipleLists;
            if (args.empty())
            {
                handlelistContents(fs.getCWD_S()->getContentNames());
            }
            else if (args.size() == 1)
            {
                // Case 2: 'ls FolderA'
                auto traverseTree_ = fs.traverseTree_S(args[0]);
                destination = fs.traverseTree_S(args[0]).unwrap();
                if (traverseTree_.isErr())
                {
                    displayError(
                        traverseTree_.unwrapErr().message,
                        traverseTree_.unwrapErr().suggestion);
                }
                handlelistContents(destination->getContentNames());
            }
            else if (args.size() > 1)
            {
                for (size_t i = 0; i < args.size(); i++)
                {
                    auto traverseTree_ = fs.traverseTree_S(args[i]);
                    if (traverseTree_.isErr())
                    {
                        displayError(
                            traverseTree_.unwrapErr().message,
                            traverseTree_.unwrapErr().suggestion);
                        continue;
                    }
                    destination = traverseTree_.unwrap();
                    multipleLists[args[i]] = destination->getContentNames();
                }
            }
            if (!multipleLists.empty())
            {
                handleMultipleListContents(multipleLists);
            }
        }
        break;
        case CommandCode::CD:
        {
            if (args.empty())
            {
                args.push_back("~");
            }
            auto traverseTree_ = fs.traverseTree_S(args[0]);
            if (traverseTree_.isErr())
            {
                displayError(
                    traverseTree_.unwrapErr().message,
                    traverseTree_.unwrapErr().suggestion);
                break;
            }
            shared_ptr<virtualFolder> destination = traverseTree_.unwrap();
            fs.setCWD(destination);
        }
        break;
        case CommandCode::TOUCH:
        {
            vector<string> rn;
            shared_ptr<virtualFolder> destination;
            auto syntaxCheck_touch_ = Validator::syntaxCheckerTOUCH(args);
            if (syntaxCheck_touch_.isErr())
            {
                displayError(
                    syntaxCheck_touch_.unwrapErr().message,
                    syntaxCheck_touch_.unwrapErr().suggestion);
                break;
            }
            for (string str : args)
            {
                vector<string> parts = parseInputs(str, '/');
                vector<string> destinationPath(parts.begin(), parts.end() - 1);
                string fileName = parts.back();

                string input = destinationPath.empty() ? "." : joinStrings(destinationPath, '/');
                auto traverseTree_ = fs.traverseTree_S(input);

                if (traverseTree_.isErr())
                {
                    displayError(
                        traverseTree_.unwrapErr().message,
                        traverseTree_.unwrapErr().suggestion);
                    continue;
                }
                shared_ptr<virtualFolder> destination = traverseTree_.unwrap();

                auto isValidFileName_ = Validator::isValidFileName(fileName);
                if (isValidFileName_.isErr())
                {
                    displayError(
                        isValidFileName_.unwrapErr().message,
                        isValidFileName_.unwrapErr().suggestion);
                    continue;
                }
                rn = parseInputs(fileName, '.');
                auto createdFile_ = destination->createFile("", fileName, rn[1]);
                if (createdFile_.isErr())
                {
                    displayError(
                        createdFile_.unwrapErr().message,
                        createdFile_.unwrapErr().suggestion);
                    continue;
                }
                else
                {
                    displaySpecialMessage(fileName + " File was created");
                }
            }
        }
        break;
        case CommandCode::RM:
        {
            shared_ptr<virtualFolder> destination;
            for (string str : args)
            {
                vector<string> parts = parseInputs(str, '/');
                vector<string> destinationPath(parts.begin(), parts.end() - 1);
                string fileName = parts.back();

                string input = destinationPath.empty() ? "." : joinStrings(destinationPath, '/');
                auto traverseTree_ = fs.traverseTree_S(input);
                if (traverseTree_.isErr())
                {
                    displayError(
                        traverseTree_.unwrapErr().message,
                        traverseTree_.unwrapErr().suggestion);
                    continue;
                }

                destination = traverseTree_.unwrap();

                auto filePtr_ = destination->getPointerFromNameAsFile(fileName);
                if (filePtr_.isErr())
                {
                    displayError(
                        filePtr_.unwrapErr().message,
                        filePtr_.unwrapErr().suggestion);
                    continue;
                }
                shared_ptr<virtualFile> filePtr = filePtr_.unwrap();

                auto removedFile_ = destination->removeFile(filePtr);
                if (removedFile_.isErr())
                {
                    displayError(
                        removedFile_.unwrapErr().message,
                        removedFile_.unwrapErr().suggestion);
                    continue;
                }
                else
                {
                    displaySpecialMessage(fileName + " File was deleted");
                }
            }
        }
        break;
        case CommandCode::CAT:
        {
        }
        break;
        case CommandCode::HELP:
            break;
        case CommandCode::EXIT:
            break;
        default:
            for (size_t i = 0; i < parts.size(); i++)
                cout << parts[i];
            cout << endl
                 << parts[0] << endl;
            cout << "Unknown command: " << cmd << ". Type 'help' for a list of commands." << endl;
            break;
        }
        if (code != CommandCode::EXIT)
        {
            displayCurrentPath(fs.getCWD_S()->buildAncestorsList(fs.getCWD()));
        }
    }
}

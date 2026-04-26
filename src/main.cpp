#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "../include/files.h"
#include "../include/display.h"
#include "../include/utils.h"
#include "../include/validation.h"

using namespace std;

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

int main()
{
    FileSystem fs;
    Validator INPUT_VALIDATOR;

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

    displayCurrentPath(fs.getCWD()->buildAncestorsList(fs.getCWD()));
    while (code != CommandCode::EXIT && getline(cin, userChoice))
    {
        vector<string> parts = parseInputs(userChoice, ' ');
        if (!INPUT_VALIDATOR.syntaxCheckerInput(parts))
        {
            displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
            displayCurrentPath(fs.getCWD()->buildAncestorsList(fs.getCWD()));
            continue;
        }

        string cmd = parts[0];

        vector<string> args(parts.begin() + 1, parts.end());

        code = commandMap.count(cmd) ? commandMap[cmd] : CommandCode::UNKNOWN;

        switch (code)
        {
        case CommandCode::MKDIR:
        {
            if (!INPUT_VALIDATOR.syntaxCheckerMKDIR(args))
            {
                displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                break;
            }
            for (string str : args)
            {
                vector<string> parts = parseInputs(str, '/');
                vector<string> destinationPath(parts.begin(), parts.end() - 1);
                string folderName = parts.back();
                virtualFolder *destination = fs.traverseTree(destinationPath.empty() ? "." : joinStrings(destinationPath, '/'), INPUT_VALIDATOR);
                if (destination == nullptr)
                {
                    displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                    continue;
                }
                if (!INPUT_VALIDATOR.isValidName(folderName))
                {
                    displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                    continue;
                }
                if (destination->createFolder(folderName, INPUT_VALIDATOR) == nullptr)
                {
                    displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
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
            virtualFolder *destination;
            map<string, vector<string>> multipleLists;
            bool checkIfMultiple = false;
            if (args.empty())
            {
                handlelistContents(fs.getCWD()->getContentNames());
            }
            else if (args.size() == 1)
            {
                // Case 2: 'ls FolderA'
                destination = fs.traverseTree(args[0], INPUT_VALIDATOR);
                if (destination != nullptr)
                {
                    handlelistContents(destination->getContentNames());
                }
                else
                {
                    displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                }
            }
            else if (args.size() > 1)
            {
                for (int i = 0; i < args.size(); i++)
                {
                    destination = fs.traverseTree(args[i], INPUT_VALIDATOR);
                    if (destination == nullptr)
                    {
                        displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                        continue;
                    }
                    else
                    {
                        multipleLists[args[i]] = destination->getContentNames();;
                    }
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
            if(fs.traverseTree(args[0], INPUT_VALIDATOR) == nullptr){
                displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                break;
            }
            fs.setCWD(fs.traverseTree(args[0], INPUT_VALIDATOR));
        }   
        break;
        case CommandCode::TOUCH:
        {
            vector<string> rn;
            virtualFolder *destination;
            if (!INPUT_VALIDATOR.syntaxCheckerTOUCH(args))
            {
                displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                break;
            }
            for (string str : args)
            {
                vector<string> parts = parseInputs(str, '/');
                vector<string> destinationPath(parts.begin(), parts.end() - 1);
                string fileName = parts.back();

                virtualFolder *destination = fs.traverseTree(destinationPath.empty() ? "." : joinStrings(destinationPath, '/'), INPUT_VALIDATOR);

                if (destination == nullptr)
                {
                    displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                    continue;
                }
                if (!INPUT_VALIDATOR.isValidFileName(fileName))
                {
                    displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                    continue;
                }
                rn = parseInputs(fileName, '.');
                if (destination->createFile("", fileName, rn[1], INPUT_VALIDATOR) == nullptr)
                {
                    displayError(INPUT_VALIDATOR.getErrorMessage(), INPUT_VALIDATOR.getSuggestion());
                }
                else
                {
                    displaySpecialMessage(fileName + " File was created");
                }
            }
        }
        break;
        case CommandCode::RM:
            break;
        case CommandCode::CAT:
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
            displayCurrentPath(fs.getCWD()->buildAncestorsList(fs.getCWD()));
        }
    }
}

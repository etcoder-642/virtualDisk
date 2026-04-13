#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "../include/files.h"
#include "../include/display.h"
#include "../include/utils.h"

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
    auto root = make_shared<virtualFolder>("root", nullptr);
    virtualFolder *cwd = root.get();

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

    displayCurrentPath(cwd->buildAncestorsList(cwd));
    while (code != CommandCode::EXIT && getline(cin, userChoice))
    {
        vector<string> parts = parseInputs(userChoice, ' ');
        if (parts.empty())
        {
            displayCurrentPath(cwd->buildAncestorsList(cwd));
            continue;
        }

        string cmd = parts[0];

        vector<string> args(parts.begin() + 1, parts.end());

        code = commandMap.count(cmd) ? commandMap[cmd] : CommandCode::UNKNOWN;

        switch (code)
        {
        case CommandCode::MKDIR:
        {
            auto folderptr = cwd->createFolder(args[0]);
            displaySpecialMessage(args[0] + " Folder Created successfully!!!");
        }
        break;
        case CommandCode::LS:
        {
            vector<string> list = cwd->getContentNames();
            handlelistContents(list);
        }
        break;
        case CommandCode::CD:
        {
            vector<string> actionList = parseInputs(args[0], '/');
            for (int i = 0; i < actionList.size(); i++)
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
                    displayCurrentPath(cwd->buildAncestorsList(cwd));
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
        }
        break;
        case CommandCode::TOUCH:
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
            for (int i = 0; i < parts.size(); i++)
                cout << parts[i];
            cout << endl
                 << parts[0] << endl;
            cout << "Unknown command: " << cmd << ". Type 'help' for a list of commands." << endl;
            break;
        }
        if (code != CommandCode::EXIT)
        {
            displayCurrentPath(cwd->buildAncestorsList(cwd));
        }
    }
}

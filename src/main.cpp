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

int
main()
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
        {"concatenate", CommandCode::CAT}
    };
    string userChoice;
    initialPage();

    cout << "~/home$ ";
    while (getline(cin, userChoice))
    {
        vector<string> parts = parseInputs(userChoice);
        if (parts.empty())
            continue;

        string cmd = parts[0];

        vector<string> args(parts.begin() + 1, parts.end());

        CommandCode code = commandMap.count(cmd) ? commandMap[cmd] : CommandCode::UNKNOWN;

    switch (code)
        {
        case CommandCode::MKDIR:
        {
            auto folderptr = cwd->createFolder(args[0]);
            displaySpecialMessage(args[0] + " Folder Created successfully!!!");
        }
        break;
        case CommandCode::LS:
            break;
        case CommandCode::CD:
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
            for(int i = 0; i < parts.size(); i++) cout << parts[i];
            cout << endl << parts[0] << endl;
            cout << "Unknown command: " << cmd << ". Type 'help' for a list of commands." << endl;
            break;
        }
    }
}
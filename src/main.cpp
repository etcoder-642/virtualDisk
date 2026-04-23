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
    auto root = fs.getRoot();
    virtualFolder *cwd = fs.getCWD();
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

    displayCurrentPath(cwd->buildAncestorsList(cwd));
    while (code != CommandCode::EXIT && getline(cin, userChoice))
    {
        vector<string> parts = parseInputs(userChoice, ' ');
        if (!INPUT_VALIDATOR.syntaxCheckerInput(parts))
        {
            displayError(INPUT_VALIDATOR.getErrorMessage());
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
            for (string str: args)
            {
                cwd->createFolder(str);
                displaySpecialMessage(str + " Folder Created successfully!!!");
            }
        }
        break;
        case CommandCode::LS:
        {
            vector<string> list;
            if(args.empty()){
                list = cwd->getContentNames();
            }else {
                if(cwd->checkFolderExistence(args[0])){

                }else {
                    displaySpecialMessage("");
                }
            }
            handlelistContents(list);
        }
        break;
        case CommandCode::CD:
        {
            cwd = static_cast<virtualFolder *>(fs.changeDirectory(args[0]));
            fs.setCWD(cwd);
        }
        break;
        case CommandCode::TOUCH:
        {
            vector<string> rn;
            for(string str: args){
                rn = parseInputs(str, '.');
                if(virtualFile::checkFileTypeExistence(rn[1])){
                    cwd->createFile("", rn[0], rn[1]);
                    displaySpecialMessage(str + "File was created");
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
            displayCurrentPath(cwd->buildAncestorsList(cwd));
        }
    }
}

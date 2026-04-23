#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "../include/files.h"
#include "../include/utils.h"
#include "../include/validation.h"

using namespace std;

vector<string> Validator::allCommands = {
"mkdir",
"ls",
"cd",
"touch",
"rm",
"cat",
"help",
"exit",

"create-directory",
"list",
"change-directory",
"create-file",
"remove",
"concatenate"
};

bool Validator::checkEmpty(vector<string> list) { return list.empty() ? false : true; }
bool Validator::syntaxCheckerInput(vector<string>& str)
{
    if(str.empty()){
        ERROR_MESSAGE = "Error:No Command Provided";
        SUGGESTION = "Type `help` to see list of all commands.";
        return false;
    }
    int checkCommand = 0;
    for(size_t i = 0; i < allCommands.size(); i++){
        if(allCommands[i] == str[0]) checkCommand++;
    }
    if(!checkCommand){
        ERROR_MESSAGE = "Error: Command Not Recognized";
        SUGGESTION = "Type `help` to see list of all commands.";
        return false;
    }
    return true;
}

bool Validator::syntaxCheckerMKDIR(vector<string>& str)
{
    if(str.empty()){
        ERROR_MESSAGE = "Error: Not enough Arguments Provided.";
        SUGGESTION = "Provide folder names to create a folder.";
        return false;
    }
    return true;
}

bool Validator::isValidName(const std::string& name) 
{
    // 1. Check for empty string
    if (name.empty()) {
        ERROR_MESSAGE = "Error: Name cannot be empty.";
        return false;
    }

    // 2. Check Length (Max 255 characters)
    if (name.length() > 255) {
        ERROR_MESSAGE = "Error: Name is too long (Max 255 chars).";
        return false;
    }

    // 3. Check for forbidden characters (Space, /, \)
    // find_first_of is faster than multiple find() calls
    if (name.find_first_of(" /\\") != std::string::npos) {
        ERROR_MESSAGE = "Error: Name contains illegal characters (space, / or \\).";
        return false;
    }

    // 4. Check for leading symbols (-, ., +)
    // We only check the very first character (index 0)
    char firstChar = name[0];
    if (firstChar == '-' || firstChar == '.' || firstChar == '+') {
        ERROR_MESSAGE = "Error: Name cannot start with a symbol (-, ., or +).";
        return false;
    }

    return true; // Passed all tests!
}
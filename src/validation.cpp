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

// checks for the validity of the syntax of the mkdir command.
bool Validator::syntaxCheckerMKDIR(vector<string>& str)
{
    if(str.empty()){
        ERROR_MESSAGE = "Error: Not enough Arguments Provided.";
        SUGGESTION = "Provide folder names to create a folder.";
        return false;
    }
    return true;
}

// Checks for Validity of file and folder Names

bool Validator::isValidName(const std::string& name) 
{
    // 1. Check for empty string
    if (name.empty()) {
        ERROR_MESSAGE = "Error: Name cannot be empty.";
        SUGGESTION = "Provide a valid name for the file or folder.";
        return false;
    }

    // 2. Check Length (Max 255 characters)
    if (name.length() > 255) {
        ERROR_MESSAGE = "Error: Name is too long (Max 255 chars).";
        SUGGESTION = "Provide a shorter name for the file or folder.";
        return false;
    }

    // 3. Check for forbidden characters (Space, /, \)
    // find_first_of is faster than multiple find() calls
    if (name.find_first_of(" /\\") != std::string::npos) {
        ERROR_MESSAGE = "Error: Name contains illegal characters (space, / or \\).";
        SUGGESTION = "Remove spaces and characters like / or \\ from the name.";
        return false;
    }

    // 4. Check for leading symbols (-, ., +)
    // We only check the very first character (index 0)
    char firstChar = name[0];
    if (firstChar == '-' || firstChar == '.' || firstChar == '+') {
        ERROR_MESSAGE = "Error: Name cannot start with a symbol (-, ., or +).";
        SUGGESTION = "Provide a valid name for the file or folder.";
        return false;
    }

    return true; // Passed all tests!
}

bool Validator::syntaxCheckerTOUCH(vector<string>& args)
{
    if(args.empty()){
        ERROR_MESSAGE = "Error: Not enough Arguments Provided.";
        SUGGESTION = "Provide file names to create a file.";
        return false;
    }else if(args[0] == "." && args.size() == 1){
        ERROR_MESSAGE = "Error: Invalid file name.";
        SUGGESTION = "Provide a valid name for the file.";
        return false;
    }
    return true;
}

bool Validator::isValidFileName(string str)
{
    vector<string> parts = parseInputs(str, '.');
    if(parts.size() == 1){
        ERROR_MESSAGE = "Error: No file type provided.";
        SUGGESTION = "type `help -ls filetype` to see list of all valid file types.";
        return false;
    }else if(parts.size() > 2){
        ERROR_MESSAGE = "Error: Invalid file name format.";
        SUGGESTION = "File names should be in the format `name.type`.";
        return false;
    }
    if(!virtualFile::checkFileTypeExistence(parts[1])){
        ERROR_MESSAGE = "Error: Invalid file type.";
        SUGGESTION = "type `help -ls filetype` to see list of all valid file types.";
        return false;
    }
    if(isValidName(parts[0]) == false){
        return false;
    }
    return true;
}
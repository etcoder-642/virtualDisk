#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "../include/files.h"
#include "../include/utils.h"
#include "../include/validation.h"
#include "../include/error/error.h"
#include "../include/error/result.h"

using namespace std;

namespace vfs
{
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

    Result<void> Validator::checkEmpty(vector<string> list) 
    { 
        string err_message_ = "Error: No Arguments Provided";
        string err_suggestion_ = "Provide at least one argument.";
        return list.empty() ? Result<void>::Err(err_message_, err_suggestion_) : Result<void>::Ok(); 
    }

    Result<void> Validator::syntaxCheckerInput(vector<string> &str)
    {
        string err_message_;
        string err_suggestion_;
        if (str.empty())
        {
            err_message_ = "Error:No Command Provided";
            err_suggestion_ = "Type `help` to see list of all commands.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }
        int checkCommand = 0;
        for (size_t i = 0; i < allCommands.size(); i++)
        {
            if (allCommands[i] == str[0])
                checkCommand++;
        }
        if (!checkCommand)
        {
            err_message_ = "Error: Invalid Command";
            err_suggestion_ = "Type `help` to see list of all commands.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }
        return Result<void>::Ok();
    }

    // checks for the validity of the syntax of the mkdir command.
    Result<void> Validator::syntaxCheckerMKDIR(vector<string> &str)
    {
        string err_message_;
        string err_suggestion_;
        if (str.empty())
        {
            err_message_ = "Error: No Arguments Provided";
            err_suggestion_ = "Provide at least one argument.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }
        return Result<void>::Ok();
    }

    // Checks for Validity of file and folder Names

    Result<void> Validator::isValidName(const string &name)
    {
        string err_message_;
        string err_suggestion_;
        // 1. Check for empty string
        if (name.empty())
        {
            err_message_ = "Error: Name cannot be empty.";
            err_suggestion_ = "Provide a valid name for the file or folder.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }

        // 2. Check Length (Max 255 characters)
        if (name.length() > 255)
        {
            err_message_ = "Error: Name is too long (Max 255 chars).";
            err_suggestion_ = "Provide a shorter name for the file or folder.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }

        // 3. Check for forbidden characters (Space, /, \)
        // find_first_of is faster than multiple find() calls
        if (name.find_first_of(" /\\") != string::npos)
        {
            err_message_ = "Error: Name contains illegal characters (space, / or \\).";
            err_suggestion_ = "Remove spaces and characters like / or \\ from the name.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }

        // 4. Check for leading symbols (-, ., +)
        // We only check the very first character (index 0)
        char firstChar = name[0];
        if (firstChar == '-' || firstChar == '.' || firstChar == '+')
        {
            err_message_ = "Error: Name cannot start with a symbol (-, ., or +).";
            err_suggestion_ = "Provide a valid name for the file or folder.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }

        return Result<void>::Ok();
    }

    Result<void> Validator::syntaxCheckerTOUCH(vector<string> &args)
    {
        string err_message_;
        string err_suggestion_;
        if (args.empty())
        {
            err_message_ = "Error: Not enough Arguments Provided.";
            err_suggestion_ = "Provide file names to create a file.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }
        else if (args[0] == "." && args.size() == 1)
        {
            err_message_ = "Error: Invalid file name.";
            err_suggestion_ = "Provide a valid name for the file.";
            return Result<void>::Err(err_message_, err_suggestion_);
        }
        return Result<void>::Ok();
    }

    Result<void> Validator::isValidFileName(string str)
    {
        vector<string> parts = parseInputs(str, '.');
        if (parts.size() == 1)
        {
            return Result<void>::Err(
                "Error: No file type provided.", 
                "type `help -ls filetype` to see list of all valid file types."
            );
        }
        else if (parts.size() > 2)
        {
            return Result<void>::Err(
                "Error: Invalid file name format.",
                 "File names should be in the format `name.type`."
            );
        }
        if (!virtualFile::checkFileTypeExistence(parts[1]))
        {
            return Result<void>::Err(
                "Error: Invalid file type.", 
                "type `help -ls filetype` to see list of all valid file types."
            );
        }
        return isValidName(parts[0]);
    }
}

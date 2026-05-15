#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <iostream>
#include <vector>

#include "error/error.h"
#include "error/result.h"

namespace vfs
{
    class Validator
    {
    private:
        static std::vector<std::string> allCommands;

    public:
        static Result<void> checkEmpty(const std::vector<std::string> list);
        static Result<void> isValidName(const std::string &name);
        static Result<void> syntaxCheckerMKDIR(std::vector<std::string> &str);
        static Result<void> syntaxCheckerInput(std::vector<std::string> &str);
        static Result<void> syntaxCheckerTOUCH(std::vector<std::string> &str);
        static Result<void> isValidFileName(std::string str);
    };
}

#endif // VALIDATION_H
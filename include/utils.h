#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

namespace vfs
{
    std::vector<std::string> parseInputs(std::string & input, char identifier);
    std::string joinStrings(const std::vector<std::string> &input, char identifier);
}

#endif
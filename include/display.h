#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "files.h"

namespace vfs
{
    void initialPage();
    void displaySpecialMessage(const std::string &message);
    void handlelistContents(const std::vector<std::string> &names);
    void displayCurrentPath(const std::vector<std::string> &ancestors);
    void displayError(const std::string &str, const std::string &suggestion);
    void handleMultipleListContents(const std::map<std::string, std::vector<std::string>> &multipleLists);
}

#endif
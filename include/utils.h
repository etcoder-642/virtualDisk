#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <vector>

using namespace std;
vector<string> parseInputs(string input, char identifier);
shared_ptr<FileSystemEntity> changeDirectory(string path, virtualFolder* cwd);

#endif
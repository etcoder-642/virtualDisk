#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>

#include "../include/files.h"
#include "../include/utils.h"

using namespace std;

bool validator::checkEmpty(vector<string> list) { return list.empty() ? false : true; }
string validator::syntaxCheckerInput(vector<string> str)
{
    if(str.empty()){
        ERROR_MESSAGE = "No Command Provided"
    }
}
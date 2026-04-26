#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "files.h"
using namespace std;

void initialPage();
void displaySpecialMessage(string message);
void handlelistContents(vector<string> names);
void displayCurrentPath(vector<string> ancestors);
void displayError(string str, string suggestion);
void handleMultipleListContents(map<string, vector<string>> multipleLists);

#endif
#ifndef DISPLAY_H
#define DISPLAY_H

#include <iostream>
#include <string>
#include <vector>

#include "files.h"
using namespace std;

void initialPage();
void displaySpecialMessage(string message);
void handlelistContents(vector<string> names);
void displayCurrentPath(vector<string> ancestors);

#endif
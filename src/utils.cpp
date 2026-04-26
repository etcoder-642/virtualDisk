#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#include "../include/files.h"
using namespace std;

vector<string> parseInputs(string input, char identifier)
{
    vector<string> res;
    string r = "";

    for (size_t i = 0; i < input.size(); i++)
    {
        if (input[i] != identifier)
        {
            // It's a real character, just keep building the word
            r += input[i];
        }
        else
        {
            // We hit a space! If we have a word in 'r', save it.
            if (!r.empty())
            {
                res.push_back(r);
                r = "";
            }
        }
    }

    // THE MOST IMPORTANT PART:
    // After the loop, if 'r' still has letters (the last word), push it!
    if (!r.empty())
    {
        res.push_back(r);
    }

    return res;
}

string joinStrings(const vector<string>& input, char identifier)
{
    string res = "";
    for(string str: input)
    {
        res += str;
        res += identifier;
    }
    return res;
}


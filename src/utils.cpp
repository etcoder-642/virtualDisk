#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

using namespace std;

vector<string> parseInputs(string input, char identifier)
{
    vector<string> res;
    string r = "";

    for (int i = 0; i < input.size(); i++)
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

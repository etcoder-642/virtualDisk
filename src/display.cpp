#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

#include "../include/display.h"

    using namespace std;

namespace vfs
{

    void initialPage()
    {
        cout << string(80, '=') << endl;
        cout << "====================== VIRTUAL DISK ===========================================" << endl;
        cout << "Hint: Type 'help' to see a list of disk operations (mkdir, ls, cd)." << endl;
        cout << string(80, '-') << endl;
        // cout << "~/home$ ";
        // getline(cin, userChoice);
        cout << endl;
    }

    void handlelistContents(const vector<string> &names)
    {
        cout << string(80, '-') << endl;
        for (size_t i = 0; i < names.size(); i++)
        {
            cout << names[i] << " ";
        }
        cout << endl
             << string(80, '-') << endl;
    }

    void handleMultipleListContents(const map<string, vector<string>> &multipleLists)
    {
        for (auto const &element : multipleLists)
        {
            string key = element.first;
            const vector<string> &value = element.second;
            cout << key << ":" << endl;
            handlelistContents(value);
            cout << endl;
        }
    }

    void displaySpecialMessage(const string &message)
    {
        cout << message << endl;
    }

    void displayCurrentPath(const vector<string> &ancestors)
    {
        string path = "~";
        for (string ancestor : ancestors)
        {
            path += "/" + ancestor;
        }
        cout << path << "$ ";
    }

    void displayError(const string &str, const string &suggestion)
    {
        cout << str << endl;
        cout << suggestion << endl;
    }

}

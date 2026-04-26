#include <iostream>
#include <iomanip>
#include <vector>
#include <string>
#include <map>

using namespace std;

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

void handlelistContents(vector<string> names)
{
    cout << string(80, '-') << endl;
    for (int i = 0; i < names.size(); i++)
    {
        cout << names[i] << " ";
    }
    cout << endl << string(80, '-') << endl;
}

void handleMultipleListContents(map<string, vector<string>> multipleLists)
{
    for(auto const& element: multipleLists)
    {
        string key = element.first;
        vector<string> value = element.second;
        cout << key << ":" << endl;
        handlelistContents(value);
        cout << endl;
    }
}

void displaySpecialMessage(string message)
{
    cout << message << endl;
}

void displayCurrentPath(vector<string> ancestors)
{
    string path = "~";
    for (string ancestor : ancestors)
    {
        path += "/" + ancestor;
    }
    cout << path << "$ ";
}

void displayError(string str, string suggestion)
{
    cout << str << endl;
    cout << suggestion << endl;
}
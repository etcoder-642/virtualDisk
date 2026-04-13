#include <iostream>
#include <iomanip>
#include <vector>
#include <string>

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
    cout << "-------------------------- LIST OF ALL FILES AND FOLDERS -----------------------" << endl;
    for (int i = 0; i < names.size(); i++)
    {
        cout << left << setw(5) << i << left << setw(30) << names[i] << string(30, '-') << endl;
    }
}

void displaySpecialMessage(string message)
{
    cout << string(80, '=') << endl;
    cout << message << endl;
    cout << string(80, '=') << endl;
    cout << endl;
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

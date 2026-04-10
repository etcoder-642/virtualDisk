#include <iostream>
#include <iomanip>


using namespace std;

void initialPage(string& userChoice) {
    cout << string(80, '=') << endl;
    cout << "====================== VIRTUAL DISK ===========================================" << endl;
    cout << "Hint: Type 'help' to see a list of disk operations (mkdir, ls, cd)." << endl;
    cout << string(80, '-') << endl;
    cout << "~/home$ ";
    cin >> userChoice;
    cout << endl;
}
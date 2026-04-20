#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <iostream>
#include <vector>

Class validator {
private:
    string ERROR_MESSAGE;
    string suggestion;
public:
    validator() : ERROR_MESSAGE("Invalid input. Please try again.") {}
    string getErrorMessage() const { return ERROR_MESSAGE; }
    void clearErrorMessage() { ERROR_MESSAGE = ""; }
    bool checkEmpty(vector<string> list);
}



#endif // VALIDATION_H
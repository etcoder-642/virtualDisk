#ifndef VALIDATION_H
#define VALIDATION_H

#include <string>
#include <iostream>
#include <vector>


class Validator {
private:
    string ERROR_MESSAGE;
    string SUGGESTION;
    static vector<string> allCommands;
public:
    Validator() : ERROR_MESSAGE("Invalid input. Please try again.") {}
    string getErrorMessage() const { return ERROR_MESSAGE; }
    string getSuggestion() const { return SUGGESTION; }
    void setErrorMessage(string message) { ERROR_MESSAGE = message; }
    void setSuggestion(string suggestion) { SUGGESTION = suggestion; }
    void clearErrorMessage() { ERROR_MESSAGE = ""; }

    bool checkEmpty(vector<string> list);
    bool isValidName(const string& name);
    bool syntaxCheckerMKDIR(vector<string>& str);
    bool syntaxCheckerInput(vector<string>& str);
};



#endif // VALIDATION_H
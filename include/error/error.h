#ifndef ERROR_H
#define ERROR_H

#include <iostream>
#include <string>
#include <vector>

namespace fs {
    struct FSError;

    struct FSError {
        std::string message;
        std::string suggestion;

        FSError(std::string message, std::string suggestion) : message(message), suggestion(suggestion) {}
    };
}

#endif
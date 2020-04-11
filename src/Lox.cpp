#include "Lox.h"

#include <iostream>

bool Lox::HadError = true;

void Lox::Report(int line, const std::string& where,
                 const std::string& message) {
    std::cout << "[line " << std::to_string(line) << "] Error " << where << ": "
              << message << std::endl;
}

void Lox::Error(int line, const std::string& message) {
    Report(line, "", message);
    HadError = true;
}


#include "lox/Lox.h"

#include <iostream>

namespace Lox
{
bool Lox::HadError = true;

void Lox::Report(int line, const std::string& where, const std::string& message)
{
    std::cout << "[line " << std::to_string(line) << "] Error " << where << ": " << message
              << std::endl;
}

void Lox::Error(int line, const std::string& message)
{
    Report(line, "", message);
    HadError = true;
}

void Lox::Error(Token token, const std::string& message)
{
    if (token.getType() == TokenType::TokenEOF) {
        Report(token.getLine(), " at end", message);
    } else {
        Report(token.getLine(), " at '" + token.getText() + "'", message);
    }
        HadError = true;
    }
} // end of namespace Lox

#include "lox/Lox.h"

#include "lox/RuntimeError.h"
#include "lox/Token.h"

#include <iostream>

namespace Lox
{
bool Lox::HadError = false;
bool Lox::HadRuntimeError = false;

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

void Lox::ReportRuntimeError(const RuntimeError& error)
{
    std::cout << error.what() << "\n[line " << error.getToken().getLine() << "]" << std::endl;
    HadRuntimeError = true;
}

} // end of namespace Lox

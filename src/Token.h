#pragma once

#include <any>
#include <string>

#include "TokenType.h"

class Token {
public:
    Token(TokenType type, std::string lexeme, std::any literal, int line);

    const std::string& getText() const { return lexeme; }

    std::string toString() const;
    std::string literalToString() const;

private:
    TokenType type;
    std::string lexeme;
    std::any literal;
    int line;
};

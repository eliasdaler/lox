#pragma once

#include <any>
#include <string>
#include <unordered_map>
#include <vector>

#include "Token.h"
#include "TokenType.h"

namespace Lox
{
class Scanner {
public:
    Scanner(std::string source);
    std::vector<Token> scanTokens();

private:
    bool isAtEnd() const;
    char advance();
    bool match(char expected);
    char peek() const;
    char peekNext() const;

    void addToken(TokenType type, std::any literal);
    void addToken(TokenType type);

    void comment();
    void string();
    void number();
    void identifier();
    void scanToken();

    // data
    std::string source;
    std::vector<Token> tokens;

    std::unordered_map<std::string, TokenType> keywords;

    int start{0};
    int current{0};
    int line{1};
};
} // end of namespace Lox


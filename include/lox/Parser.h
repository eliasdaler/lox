#pragma once

#include <stdexcept>
#include <memory>
#include <vector>

#include "Expr.h"
#include "Token.h"

namespace Lox
{
class Parser {
public:
    Parser(std::vector<Token> tokens);
    std::unique_ptr<Expr> parse();

private:
    bool check(TokenType type) const;

    template<typename... Args>
    bool match(Args... args);

    Token advance();
    Token peek() const;
    Token previous() const;
    bool isAtEnd() const;

    void synchronize();
    Token consume(TokenType type, const char* message);

    class ParseError : public std::runtime_error {
    public:
        ParseError() : std::runtime_error("") {}
    };

    ParseError error(Token token, const char* message) const;

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> addition();
    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> primary();

    // data
    std::vector<Token> tokens;
    int current{0};
};

template<typename... Args>
bool Parser::match(Args... args)
{
    std::initializer_list<bool> results{check(args)...};
    for (auto r : results) {
        if (r) {
            advance();
            return true;
        }
    }
    return false;
}

} // end of namespace Lox

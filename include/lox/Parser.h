#pragma once

#include <memory>
#include <stdexcept>
#include <vector>

#include "Expr/Expr.h"
#include "Stmt/Stmt.h"
#include "Token.h"

namespace Lox
{
class Parser {
public:
    Parser(std::vector<Token> tokens);
    std::vector<std::unique_ptr<Stmt>> parse();

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

    std::unique_ptr<Stmt> declaration();
    std::unique_ptr<Stmt> varDeclaration();
    std::unique_ptr<Stmt> function(const std::string& kind);

    std::unique_ptr<Stmt> statement();
    std::unique_ptr<Stmt> forStatement();
    std::unique_ptr<Stmt> ifStatement();
    std::unique_ptr<Stmt> printStatement();
    std::unique_ptr<Stmt> returnStatement();
    std::unique_ptr<Stmt> whileStatement();
    std::vector<std::unique_ptr<Stmt>> block();
    std::unique_ptr<Stmt> expressionStatement();

    std::unique_ptr<Expr> expression();
    std::unique_ptr<Expr> assignment();
    std::unique_ptr<Expr> orExpression();
    std::unique_ptr<Expr> andExpression();
    std::unique_ptr<Expr> equality();
    std::unique_ptr<Expr> comparison();
    std::unique_ptr<Expr> addition();
    std::unique_ptr<Expr> multiplication();
    std::unique_ptr<Expr> unary();
    std::unique_ptr<Expr> call();
    std::unique_ptr<Expr> finishCall(std::unique_ptr<Expr> callee);
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

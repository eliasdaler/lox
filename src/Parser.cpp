#include "lox/Parser.h"

#include "lox/Expr/AssignExpr.h"
#include "lox/Expr/BinaryExpr.h"
#include "lox/Expr/CallExpr.h"
#include "lox/Expr/GroupingExpr.h"
#include "lox/Expr/LiteralExpr.h"
#include "lox/Expr/LogicalExpr.h"
#include "lox/Expr/UnaryExpr.h"
#include "lox/Expr/VarExpr.h"

#include "lox/Stmt/BlockStmt.h"
#include "lox/Stmt/ExpressionStmt.h"
#include "lox/Stmt/FunctionStmt.h"
#include "lox/Stmt/IfStmt.h"
#include "lox/Stmt/PrintStmt.h"
#include "lox/Stmt/ReturnStmt.h"
#include "lox/Stmt/VarStmt.h"
#include "lox/Stmt/WhileStmt.h"

#include "lox/Lox.h"

#include <fmt/core.h>

#include <memory>

namespace Lox
{
constexpr int MAX_FUNCTION_ARGUMENTS = 255;

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens))
{}

std::vector<std::unique_ptr<Stmt>> Parser::parse()
{
    // program → declaration* EOF ;
    std::vector<std::unique_ptr<Stmt>> statements;
    while (!isAtEnd()) {
        statements.push_back(declaration());
    }

    return statements;
}

std::unique_ptr<Stmt> Parser::declaration()
{
    // declaration → varDecl | statement ;
    try {
        // funDecl → "fun" function ;
        if (match(TokenType::Fun)) {
            return function("function");
        }
        // varDecl → "var" varDeclaration;
        if (match(TokenType::Var)) {
            return varDeclaration();
        }

        return statement();
    } catch (ParseError error) {
        synchronize();
        return nullptr;
    }
}

std::unique_ptr<Stmt> Parser::varDeclaration()
{
    // varDeclaration → IDENTIFIER ("=" expression)? ";";
    auto name = consume(TokenType::Identifier, "Expect variable name");

    std::unique_ptr<Expr> initializer;
    if (match(TokenType::Equal)) {
        initializer = expression();
    }

    consume(TokenType::Semicolon, "Expect ';' after variable declaration");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::function(const std::string& kind)
{
    // function → IDENTIFIER "(" parameters? ")" block ;
    static const auto errNameMissing = fmt::format("Expect {} name", kind);
    static const auto errLeftParenMissing = fmt::format("Expect '(' after {} name", kind);
    static const auto errLeftBraceMissing = fmt::format("Expect '{{' after {} name", kind);

    auto name = consume(TokenType::Identifier, errNameMissing.c_str());

    consume(TokenType::LeftParen, errLeftParenMissing.c_str());

    std::vector<Token> parameters;
    if (!check(TokenType::RightParen)) {
        do {
            if (parameters.size() >= MAX_FUNCTION_ARGUMENTS) {
                static const auto errMsg =
                    fmt::format("Can't have more than {} arguments", MAX_FUNCTION_ARGUMENTS);
                error(peek(), errMsg.c_str());
            }

            parameters.push_back(consume(TokenType::Identifier, "Expect parameter name"));
        } while (match(TokenType::Comma));
    }
    consume(TokenType::RightParen, "Expect ')' after parameters.");

    consume(TokenType::LeftBrace, errLeftBraceMissing.c_str());

    auto body = block();
    return std::make_unique<FunctionStmt>(name, std::move(parameters), std::move(body));
}

std::unique_ptr<Stmt> Parser::statement()
{
    // statement → exprStmt
    //             | forStmt
    //             | ifStmt
    //             | printStmt
    //             | returnStmt
    //             | whileStmt
    //             | block;
    if (match(TokenType::For)) {
        return forStatement();
    }

    if (match(TokenType::If)) {
        return ifStatement();
    }

    if (match(TokenType::Print)) {
        return printStatement();
    }

    if (match(TokenType::Return)) {
        return returnStatement();
    }

    if (match(TokenType::While)) {
        return whileStatement();
    }

    if (match(TokenType::LeftBrace)) {
        return std::make_unique<BlockStmt>(block());
    }

    return expressionStatement();
}

std::unique_ptr<Stmt> Parser::forStatement()
{
    // forStmt → "for" "(" ( varDecl | exprStmt | ";" )
    //           expression? ";"
    //           expression? ")" statement ;
    consume(TokenType::LeftParen, "Expect '(' after 'for'.");

    std::unique_ptr<Stmt> initializer;
    if (match(TokenType::Semicolon)) {
        initializer = nullptr;
    } else if (match(TokenType::Var)) {
        initializer = varDeclaration();
    } else {
        initializer = expressionStatement();
    }

    std::unique_ptr<Expr> condition;
    if (!check(TokenType::Semicolon)) {
        condition = expression();
    }
    consume(TokenType::Semicolon, "Expect ';' after loop condition.");

    std::unique_ptr<Expr> increment;
    if (!check(TokenType::RightParen)) {
        increment = expression();
    }
    consume(TokenType::RightParen, "Expect ')' after for clauses.");

    auto body = statement();

    // for (<init>; <cond>; <iter>) <body> is equvivalent to:
    //
    //     <init>
    //     while (<cond>) {
    //         <body>
    //         <iter>
    //      }

    // increment is executed after body on each iteration
    if (increment) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(body));
        statements.push_back(std::make_unique<ExpressionStmt>(std::move(increment)));
        body = std::make_unique<BlockStmt>(std::move(statements));
    }

    if (!condition) {
        condition = std::make_unique<LiteralExpr>(true);
    }
    body = std::make_unique<WhileStmt>(std::move(condition), std::move(body));

    if (initializer) {
        std::vector<std::unique_ptr<Stmt>> statements;
        statements.push_back(std::move(initializer));
        statements.push_back(std::move(body));
        body = std::make_unique<BlockStmt>(std::move(statements));
    }

    return body;
}

std::unique_ptr<Stmt> Parser::ifStatement()
{
    // ifStmt → "if" "(" expression ")" statement ( "else" statement )? ;
    consume(TokenType::LeftParen, "Expect '(' after 'if'.");
    auto condition = expression();
    consume(TokenType::RightParen, "Expect ')' after 'if'.");

    auto thenBranch = statement();
    std::unique_ptr<Stmt> elseBranch;
    if (match(TokenType::Else)) {
        elseBranch = statement();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch),
                                    std::move(elseBranch));
}

std::unique_ptr<Stmt> Parser::printStatement()
{
    // printStmt → "print" expression ";" ;
    auto value = expression();
    consume(TokenType::Semicolon, "Expect ';' after value");
    return std::make_unique<PrintStmt>(std::move(value));
}

std::unique_ptr<Stmt> Parser::returnStatement()
{
    // returnStmt → "return" expression? ";" ;
    auto keyword = previous();
    std::unique_ptr<Expr> value;
    if (!check(TokenType::Semicolon)) {
        value = expression();
    }

    consume(TokenType::Semicolon, "Expect ';' after return value");
    return std::make_unique<ReturnStmt>(std::move(keyword), std::move(value));
}

std::unique_ptr<Stmt> Parser::whileStatement()
{
    // whileStmt → "while" "(" expression ")" statement ;
    consume(TokenType::LeftParen, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RightParen, "Expect ')' after 'while'.");

    return std::make_unique<WhileStmt>(std::move(condition), statement());
}

std::vector<std::unique_ptr<Stmt>> Parser::block()
{
    // block → "{" declaration* "}" ;
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RightBrace, "Expect '}' after block.");
    return statements;
}

std::unique_ptr<Stmt> Parser::expressionStatement()
{
    // exprStmt → expression ";" ;
    auto expr = expression();
    consume(TokenType::Semicolon, "Expect ';' after expression");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

std::unique_ptr<Expr> Parser::expression()
{
    // expression → assignment ;
    return assignment();
}

std::unique_ptr<Expr> Parser::assignment()
{
    // assignment → IDENTIFIER "=" assignment | logic_or ;
    auto expr = orExpression();

    if (match(TokenType::Equal)) {
        auto equals = previous();
        auto value = assignment();

        if (auto* varExpr = dynamic_cast<VarExpr*>(expr.get()); varExpr) {
            return std::make_unique<AssignExpr>(varExpr->getName(), std::move(value));
        }

        error(equals, "Invalid assignment target");
    }

    return expr;
}

std::unique_ptr<Expr> Parser::orExpression()
{
    // logic_or → logic_and ( "or" logic_and )* ;
    auto expr = andExpression();

    while (match(TokenType::Or)) {
        auto op = previous();
        auto right = andExpression();
        expr = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::andExpression()
{
    // logic_and  → equality ( "and" equality )* ;
    auto expr = equality();

    while (match(TokenType::And)) {
        auto op = previous();
        auto right = equality();
        expr = std::make_unique<LogicalExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::equality()
{
    // equality → comparison ( ( "!=" | "==" ) comparison )* ;
    auto expr = comparison();
    while (match(TokenType::BangEqual, TokenType::EqualEqual)) {
        auto op = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::comparison()
{
    // comparison → addition ( ( ">" | ">=" | "<" | "<=" ) addition )* ;
    auto expr = addition();
    while (
        match(TokenType::Greater, TokenType::GreaterEqual, TokenType::Less, TokenType::LessEqual)) {
        auto op = previous();
        auto right = addition();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::addition()
{
    // addition → multiplication ( ( "-" | "+" ) multiplication )* ;
    auto expr = multiplication();
    while (match(TokenType::Minus, TokenType::Plus)) {
        auto op = previous();
        auto right = multiplication();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::multiplication()
{
    // multiplication → unary ( ( "/" | "*" ) unary )* ;
    auto expr = unary();
    while (match(TokenType::Slash, TokenType::Star)) {
        auto op = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), op, std::move(right));
    }

    return expr;
}

std::unique_ptr<Expr> Parser::unary()
{
    // unary → ( "!" | "-" ) unary | call;
    if (match(TokenType::Bang, TokenType::Minus)) {
        auto op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return call();
}

std::unique_ptr<Expr> Parser::call()
{
    // call → primary ( "(" arguments? ")" )* ;
    auto expr = primary();
    while (true) {
        if (match(TokenType::LeftParen)) {
            expr = finishCall(std::move(expr));
        } else {
            break;
        }
    }
    return expr;
}

std::unique_ptr<Expr> Parser::finishCall(std::unique_ptr<Expr> callee)
{
    std::vector<std::unique_ptr<Expr>> arguments;

    if (!check(TokenType::RightParen)) {
        do {
            if (arguments.size() >= MAX_FUNCTION_ARGUMENTS) {
                static const auto errMsg =
                    fmt::format("Can't have more than {} arguments", MAX_FUNCTION_ARGUMENTS);
                error(peek(), errMsg.c_str());
            }
            arguments.push_back(expression());
        } while (match(TokenType::Comma));
    }

    auto paren = consume(TokenType::RightParen, "Expect ')' after the arguments.");
    return std::make_unique<CallExpr>(std::move(callee), paren, std::move(arguments));
}

std::unique_ptr<Expr> Parser::primary()
{
    // primary        → NUMBER | STRING | "false" | "true" | "nil"
    //                  IDENTIFIER | "(" expression ")" ;
    if (match(TokenType::Number, TokenType::String)) {
        return std::make_unique<LiteralExpr>(previous().getLiteral());
    }

    if (match(TokenType::False)) {
        return std::make_unique<LiteralExpr>(false);
    }

    if (match(TokenType::True)) {
        return std::make_unique<LiteralExpr>(true);
    }

    if (match(TokenType::Nil)) {
        return std::make_unique<LiteralExpr>(std::any{});
    }

    if (match(TokenType::Identifier)) {
        return std::make_unique<VarExpr>(previous());
    }

    if (match(TokenType::LeftParen)) {
        auto expr = expression();
        consume(TokenType::RightParen, "Expect ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }

    throw error(peek(), "Expect expression.");
}

bool Parser::check(TokenType type) const
{
    if (isAtEnd()) {
        return false;
    }
    return peek().getType() == type;
}

Token Parser::advance()
{
    if (!isAtEnd()) {
        ++current;
    }
    return previous();
}

Token Parser::peek() const
{
    return tokens.at(current);
}

Token Parser::previous() const
{
    return tokens.at(current - 1);
}

bool Parser::isAtEnd() const
{
    return peek().getType() == TokenType::TokenEOF;
}

void Parser::synchronize()
{
    advance();

    while (!isAtEnd()) {
        if (previous().getType() == TokenType::Semicolon) {
            return;
        }

        switch (peek().getType()) {
        case TokenType::Class:
        case TokenType::Fun:
        case TokenType::Var:
        case TokenType::For:
        case TokenType::If:
        case TokenType::While:
        case TokenType::Print:
        case TokenType::Return:
            return;
        default:
            break;
        }

        advance();
    }
}

Token Parser::consume(TokenType type, const char* message)
{
    if (check(type)) {
        return advance();
    }

    throw error(peek(), message);
}

Parser::ParseError Parser::error(Token token, const char* message) const
{
    Lox::Error(token, message);
    throw ParseError{};
}

} // end of namespace Lox

#include "lox/Parser.h"

#include "lox/AssignExpr.h"
#include "lox/BinaryExpr.h"
#include "lox/GroupingExpr.h"
#include "lox/LiteralExpr.h"
#include "lox/LogicalExpr.h"
#include "lox/UnaryExpr.h"
#include "lox/VarExpr.h"

#include "lox/BlockStmt.h"
#include "lox/ExpressionStmt.h"
#include "lox/IfStmt.h"
#include "lox/PrintStmt.h"
#include "lox/VarStmt.h"
#include "lox/WhileStmt.h"

#include "lox/Lox.h"

namespace Lox
{
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
    auto name = consume(TokenType::Identifier, "Expect variable name");

    std::unique_ptr<Expr> initializer;
    if (match(TokenType::Equal)) {
        initializer = expression();
    }

    consume(TokenType::Semicolon, "Expect ';' after variable declaration");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

std::unique_ptr<Stmt> Parser::statement()
{
    // statement → exprStmt | ifStmt | printStmt | whileStmt | block;
    if (match(TokenType::If)) {
        return ifStatement();
    }

    if (match(TokenType::Print)) {
        return printStatement();
    }

    if (match(TokenType::While)) {
        return whileStatement();
    }

    if (match(TokenType::LeftBrace)) {
        return block();
    }

    return expressionStatement();
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

std::unique_ptr<Stmt> Parser::whileStatement()
{
    // whileStmt → "while" "(" expression ")" statement ;
    consume(TokenType::LeftParen, "Expect '(' after 'while'.");
    auto condition = expression();
    consume(TokenType::RightParen, "Expect ')' after 'while'.");

    return std::make_unique<WhileStmt>(std::move(condition), statement());
}

std::unique_ptr<Stmt> Parser::block()
{
    // block → "{" declaration* "}" ;
    std::vector<std::unique_ptr<Stmt>> statements;

    while (!check(TokenType::RightBrace) && !isAtEnd()) {
        statements.push_back(declaration());
    }

    consume(TokenType::RightBrace, "Expect '}' after block.");
    return std::make_unique<BlockStmt>(std::move(statements));
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
            auto name = varExpr->getName();
            return std::make_unique<AssignExpr>(name, std::move(value));
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
    // unary → ( "!" | "-" ) unary | primary ;
    if (match(TokenType::Bang, TokenType::Minus)) {
        auto op = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(op, std::move(right));
    }

    return primary();
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

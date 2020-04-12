#pragma once

#include <any>
#include <stdexcept>

#include "ExprVisitor.h"
#include "Token.h"

namespace Lox
{
class Expr;
class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;

class Interpreter : public ExprVisitor<std::any> {
public:
    std::any intepret(const Expr& expr);

    class RuntimeError : public std::runtime_error {
    public:
        RuntimeError(const Token& token, const std::string& message) :
            std::runtime_error(message),
            token(token)
        {}

        const Token& getToken() const { return token; }

    private:
        Token token;
    };

private:
    std::any evaluate(const Expr& expr);
    bool isTruthy(const std::any& object) const;
    bool isEqual(const std::any& left, const std::any& right) const;

    void checkNumberOperand(const Token& op, const std::any& operand) const;
    void checkNumberOperands(const Token& op, const std::any& left, const std::any& right) const;

    std::any visitBinaryExpr(const BinaryExpr& expr) override;
    std::any visitGroupingExpr(const GroupingExpr& expr) override;
    std::any visitLiteralExpr(const LiteralExpr& expr) override;
    std::any visitUnaryExpr(const UnaryExpr& expr) override;
};

} // end of namespace Lox

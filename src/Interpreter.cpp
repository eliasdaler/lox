#include "lox/Interpreter.h"

#include "lox/Lox.h"

#include "lox/BinaryExpr.h"
#include "lox/GroupingExpr.h"
#include "lox/LiteralExpr.h"
#include "lox/UnaryExpr.h"

#include <cmath>
#include <iostream>

namespace
{} // end of anonymous namespace

namespace Lox
{
std::any Interpreter::intepret(const Expr& expr)
{
    try {
        return evaluate(expr);
    } catch (RuntimeError error) {
        Lox::ReportRuntimeError(error);
    }
    return {};
}

std::any Interpreter::evaluate(const Expr& expr)
{
    return expr.accept(*this);
}

bool Interpreter::isTruthy(const std::any& object) const
{
    if (!object.has_value()) {
        return false;
    }

    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object);
    }

    return true;
}

bool Interpreter::isEqual(const std::any& left, const std::any& right) const
{
    // nil is only equal to nil
    if (!left.has_value() && !right.has_value()) {
        return true;
    }
    if (!left.has_value()) {
        return false;
    }

    if (left.type() != right.type()) {
        return false;
    }

    if (left.type() == typeid(bool)) {
        return std::any_cast<bool>(left) == std::any_cast<bool>(right);
    }

    if (left.type() == typeid(double)) {
        return std::any_cast<double>(left) == std::any_cast<double>(right);
    }

    if (left.type() == typeid(std::string)) {
        return std::any_cast<std::string>(left) == std::any_cast<std::string>(right);
    }

    return false;
}

void Interpreter::checkNumberOperand(const Token& op, const std::any& operand) const
{
    if (operand.type() == typeid(double)) {
        return;
    }

    throw Interpreter::RuntimeError(op, "Operand must be a number");
}

void Interpreter::checkNumberOperands(const Token& op, const std::any& left,
                                      const std::any& right) const
{
    if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return;
    }

    throw Interpreter::RuntimeError(op, "Operands must be numbers");
}

std::any Interpreter::visitBinaryExpr(const BinaryExpr& expr)
{
    const auto left = evaluate(expr.getLeftExpr());
    const auto right = evaluate(expr.getRightExpr());

    switch (expr.getOp().getType()) {
    // equality operators
    case TokenType::BangEqual:
        return !isEqual(left, right);
    case TokenType::Equal:
        return isEqual(left, right);
    // comparison operators
    case TokenType::Greater:
        checkNumberOperands(expr.getOp(), left, right);
        return std::any_cast<double>(left) > std::any_cast<double>(right);
    case TokenType::GreaterEqual:
        checkNumberOperands(expr.getOp(), left, right);
        return std::any_cast<double>(left) >= std::any_cast<double>(right);
    case TokenType::Less:
        checkNumberOperands(expr.getOp(), left, right);
        return std::any_cast<double>(left) < std::any_cast<double>(right);
    case TokenType::LessEqual:
        checkNumberOperands(expr.getOp(), left, right);
        return std::any_cast<double>(left) <= std::any_cast<double>(right);
    // arithmetic operators
    case TokenType::Minus:
        checkNumberOperands(expr.getOp(), left, right);
        return std::any_cast<double>(left) - std::any_cast<double>(right);
    case TokenType::Plus:
        if (left.type() == typeid(double) && right.type() == typeid(double)) {
            return std::any_cast<double>(left) + std::any_cast<double>(right);
        }

        if (left.type() == typeid(std::string) && right.type() == typeid(std::string)) {
            return std::any_cast<std::string>(left) + std::any_cast<std::string>(right);
        }

        throw Interpreter::RuntimeError(expr.getOp(),
                                        "Operands must be two numbers or two strings");
    case TokenType::Slash:
        checkNumberOperands(expr.getOp(), left, right);
        return std::any_cast<double>(left) / std::any_cast<double>(right);
    case TokenType::Star:
        checkNumberOperands(expr.getOp(), left, right);
        return std::any_cast<double>(left) * std::any_cast<double>(right);
    default:
        return std::any{}; // unreachable
    }
}

std::any Interpreter::visitGroupingExpr(const GroupingExpr& expr)
{
    return evaluate(expr.getExpr());
}

std::any Interpreter::visitLiteralExpr(const LiteralExpr& expr)
{
    return expr.getLiteral();
}

std::any Interpreter::visitUnaryExpr(const UnaryExpr& expr)
{
    const auto right = evaluate(expr.getRightExpr());
    switch (expr.getOp().getType()) {
    case TokenType::Bang:
        return !isTruthy(right);
    case TokenType::Minus:
        checkNumberOperand(expr.getOp(), right);
        return -std::any_cast<double>(right);
    default:
        return std::any{}; // unreachable
    }
}

} // end of namespace Lox

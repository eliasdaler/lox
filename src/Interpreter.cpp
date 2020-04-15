#include "lox/Interpreter.h"

#include "lox/Environment.h"
#include "lox/Lox.h"
#include "lox/RuntimeError.h"

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

#include <cassert>
#include <cmath>
#include <ostream>

namespace
{
std::string stringify(const std::any& object)
{
    if (!object.has_value()) {
        return "nil";
    }

    if (object.type() == typeid(bool)) {
        return std::any_cast<bool>(object) ? "true" : "false";
    }

    if (object.type() == typeid(double)) {
        double n = std::any_cast<double>(object);
        if (std::trunc(n) == n) { // is int
            return std::to_string((int)n);
        } else {
            return std::to_string(n); // TODO: don't print trailing zeros
        }
    }

    if (object.type() == typeid(std::string)) {
        return std::any_cast<std::string>(object);
    }

    return "";
}
} // end of anonymous namespace

namespace Lox
{
Interpreter::Interpreter(std::ostream& out) : out(out), environment(std::make_unique<Environment>())
{}

Interpreter::~Interpreter() = default;

void Interpreter::intepret(const std::vector<std::unique_ptr<Stmt>>& statements)
{
    try {
        for (const auto& ptr : statements) {
            assert(ptr != nullptr);
            execute(*ptr);
        }
    } catch (RuntimeError error) {
        Lox::ReportRuntimeError(error);
    }
}

void Interpreter::execute(const Stmt& stmt)
{
    stmt.accept(*this);
}

void Interpreter::executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements,
                               std::unique_ptr<Environment> env)
{
    auto previous = std::move(environment);
    try {
        environment = std::move(env);
        for (const auto& statementPtr : statements) {
            assert(statementPtr != nullptr);
            execute(*statementPtr);
        }
    } catch (RuntimeError error) {
        environment = std::move(previous);
        throw error;
    }

    environment = std::move(previous);
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

    throw RuntimeError(op, "Operand must be a number");
}

void Interpreter::checkNumberOperands(const Token& op, const std::any& left,
                                      const std::any& right) const
{
    if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return;
    }

    throw RuntimeError(op, "Operands must be numbers");
}

std::any Interpreter::visitAssignExpr(const AssignExpr& expr)
{
    auto value = evaluate(expr.getValue());
    assert(environment != nullptr);
    environment->assign(expr.getName(), value);
    return value;
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

        throw RuntimeError(expr.getOp(), "Operands must be two numbers or two strings");
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

std::any Interpreter::visitLogicalExpr(const LogicalExpr& expr)
{
    auto left = evaluate(expr.getLeftExpr());

    if (expr.getOp().getType() == TokenType::Or) {
        if (isTruthy(left)) {
            return left;
        }
    } else {
        if (!isTruthy(left)) {
            return left;
        }
    }

    return evaluate(expr.getRightExpr());
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

std::any Interpreter::visitVarExpr(const VarExpr& expr)
{
    assert(environment != nullptr);
    return environment->get(expr.getName());
}

std::any Interpreter::visitBlockStmt(const BlockStmt& stmt)
{
    executeBlock(stmt.getStatements(), std::make_unique<Environment>(environment.get()));
    return {};
}

std::any Interpreter::visitExpressionStmt(const ExpressionStmt& stmt)
{
    evaluate(stmt.getExpr());
    return {};
}

std::any Interpreter::visitIfStmt(const IfStmt& stmt)
{
    if (isTruthy(evaluate(stmt.getCondition()))) {
        execute(stmt.getThenBranch());
    } else {
        if (stmt.hasElseBranch()) {
            execute(stmt.getElseBranch());
        }
    }
    return {};
}

std::any Interpreter::visitPrintStmt(const PrintStmt& stmt)
{
    auto value = evaluate(stmt.getExpr());
    out << ::stringify(value) << std::endl;
    return {};
}

std::any Interpreter::visitVarStmt(const VarStmt& stmt)
{
    std::any value;
    if (stmt.hasInitializer()) {
        value = evaluate(stmt.getInitializer());
    }

    assert(environment != nullptr);
    environment->define(stmt.getName().getText(), value);
    return {};
}

std::any Interpreter::visitWhileStmt(const WhileStmt& stmt)
{
    while(isTruthy(evaluate(stmt.getCondition()))) {
        execute(stmt.getBody());
    }
    return {};
}

} // end of namespace Lox

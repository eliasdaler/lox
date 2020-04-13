#pragma once

#include <any>
#include <iosfwd>
#include <memory>
#include <vector>

#include "ExprVisitor.h"
#include "StmtVisitor.h"

#include "Environment.h"
#include "Token.h"

namespace Lox
{
class Expr;
class AssignExpr;
class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;

class Stmt;
class ExpressionStmt;
class PrintStmt;
class VarStmt;

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<std::any> {
public:
    Interpreter(std::ostream& out);
    void intepret(const std::vector<std::unique_ptr<Stmt>>& statements);

private:
    void execute(const Stmt& stmt);
    std::any evaluate(const Expr& expr);

    bool isTruthy(const std::any& object) const;
    bool isEqual(const std::any& left, const std::any& right) const;

    void checkNumberOperand(const Token& op, const std::any& operand) const;
    void checkNumberOperands(const Token& op, const std::any& left, const std::any& right) const;

    std::any visitAssignExpr(const AssignExpr& expr) override;
    std::any visitBinaryExpr(const BinaryExpr& expr) override;
    std::any visitGroupingExpr(const GroupingExpr& expr) override;
    std::any visitLiteralExpr(const LiteralExpr& expr) override;
    std::any visitUnaryExpr(const UnaryExpr& expr) override;
    std::any visitVarExpr(const VarExpr& expr) override;

    std::any visitExpressionStmt(const ExpressionStmt& stmt) override;
    std::any visitPrintStmt(const PrintStmt& stmt) override;
    std::any visitVarStmt(const VarStmt& stmt) override;

    // data
    Environment environment;
    std::ostream& out;
};

} // end of namespace Lox

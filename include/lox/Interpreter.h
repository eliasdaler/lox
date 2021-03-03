#pragma once

#include <any>
#include <iosfwd>
#include <memory>
#include <vector>

#include "Expr/ExprVisitor.h"
#include "Stmt/StmtVisitor.h"

#include "Token.h"

namespace Lox
{
class Environment;

class Expr;
class Stmt;

class Interpreter : public ExprVisitor<std::any>, public StmtVisitor<std::any> {
public:
public:
    Interpreter(std::ostream& out);
    ~Interpreter(); // for forward declaration of Environment
    void intepret(const std::vector<std::unique_ptr<Stmt>>& statements);

    Environment& getGlobalsEnvironment();

    void execute(const Stmt& stmt);
    void executeBlock(const std::vector<std::unique_ptr<Stmt>>& statements,
                      std::unique_ptr<Environment> env);
    std::any evaluate(const Expr& expr);

private:
    bool isTruthy(const std::any& object) const;
    bool isEqual(const std::any& left, const std::any& right) const;

    void checkNumberOperand(const Token& op, const std::any& operand) const;
    void checkNumberOperands(const Token& op, const std::any& left, const std::any& right) const;

    std::any visitAssignExpr(const AssignExpr& expr) override;
    std::any visitBinaryExpr(const BinaryExpr& expr) override;
    std::any visitCallExpr(const CallExpr& expr) override;
    std::any visitGroupingExpr(const GroupingExpr& expr) override;
    std::any visitLogicalExpr(const LogicalExpr& expr) override;
    std::any visitLiteralExpr(const LiteralExpr& expr) override;
    std::any visitUnaryExpr(const UnaryExpr& expr) override;
    std::any visitVarExpr(const VarExpr& expr) override;

    std::any visitBlockStmt(const BlockStmt& stmt) override;
    std::any visitExpressionStmt(const ExpressionStmt& stmt) override;
    std::any visitIfStmt(const IfStmt& stmt) override;
    std::any visitFunctionStmt(const FunctionStmt& stmt) override;
    std::any visitPrintStmt(const PrintStmt& stmt) override;
    std::any visitReturnStmt(const ReturnStmt& stmt) override;
    std::any visitVarStmt(const VarStmt& stmt) override;
    std::any visitWhileStmt(const WhileStmt& stmt) override;

    // data
    std::unique_ptr<Environment> globals;
    Environment* globalEnvironment;
    std::unique_ptr<Environment> environment; // current environment

    class EnterEnviromentGuard {
    public:
        EnterEnviromentGuard(Interpreter& i, std::unique_ptr<Environment> env);
        ~EnterEnviromentGuard();

    private:
        Interpreter& i;
        std::unique_ptr<Environment> previous;
    };

    std::ostream& out;
};

} // end of namespace Lox

#pragma once

namespace Lox
{
class AssignExpr;
class BinaryExpr;
class CallExpr;
class GroupingExpr;
class LogicalExpr;
class LiteralExpr;
class UnaryExpr;
class VarExpr;

template<typename R>
class ExprVisitor {
public:
    ~ExprVisitor() = default;

    virtual R visitAssignExpr(const AssignExpr& expr) = 0;
    virtual R visitBinaryExpr(const BinaryExpr& expr) = 0;
    virtual R visitCallExpr(const CallExpr& expr) = 0;
    virtual R visitGroupingExpr(const GroupingExpr& expr) = 0;
    virtual R visitLogicalExpr(const LogicalExpr& expr) = 0;
    virtual R visitLiteralExpr(const LiteralExpr& expr) = 0;
    virtual R visitUnaryExpr(const UnaryExpr& expr) = 0;
    virtual R visitVarExpr(const VarExpr& expr) = 0;
};

} // end of namespace Lox

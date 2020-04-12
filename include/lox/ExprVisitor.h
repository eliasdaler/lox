#pragma once

namespace Lox
{
class BinaryExpr;
class GroupingExpr;
class LiteralExpr;
class UnaryExpr;

template<typename R>
class ExprVisitor {
public:
    ~ExprVisitor() = default;

    virtual R visitBinaryExpr(const BinaryExpr& expr) = 0;
    virtual R visitGroupingExpr(const GroupingExpr& expr) = 0;
    virtual R visitLiteralExpr(const LiteralExpr& expr) = 0;
    virtual R visitUnaryExpr(const UnaryExpr& expr) = 0;
};

} // end of namespace Lox

#pragma once

namespace Lox
{
class ExpressionStmt;
class PrintStmt;

template<typename R>
class StmtVisitor {
public:
    ~StmtVisitor() = default;

    virtual R visitExpressionStmt(const ExpressionStmt& stmt) = 0;
    virtual R visitPrintStmt(const PrintStmt& stmt) = 0;
};

} // end of namespace Lox

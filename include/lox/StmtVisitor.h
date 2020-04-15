#pragma once

namespace Lox
{
class BlockStmt;
class ExpressionStmt;
class IfStmt;
class PrintStmt;
class VarStmt;

template<typename R>
class StmtVisitor {
public:
    ~StmtVisitor() = default;

    virtual R visitBlockStmt(const BlockStmt& stmt) = 0;
    virtual R visitExpressionStmt(const ExpressionStmt& stmt) = 0;
    virtual R visitIfStmt(const IfStmt& stmt) = 0;
    virtual R visitPrintStmt(const PrintStmt& stmt) = 0;
    virtual R visitVarStmt(const VarStmt& stmt) = 0;
};

} // end of namespace Lox

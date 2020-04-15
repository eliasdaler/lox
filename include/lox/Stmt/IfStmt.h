#pragma once

#include <memory>

#include "Stmt.h"
#include "StmtVisitor.h"

#include "lox/Expr/Expr.h"

namespace Lox
{
class IfStmt : public Stmt {
public:
    IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch, std::unique_ptr<Stmt> elseBranch);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    const Expr& getCondition() const { return *condition; }
    const Stmt& getThenBranch() const { return *thenBranch; }
    bool hasElseBranch() const { return elseBranch != nullptr; }
    const Stmt& getElseBranch() const { return *elseBranch; }

private:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> thenBranch;
    std::unique_ptr<Stmt> elseBranch;
};

} // end of namespace Lox

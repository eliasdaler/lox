#include "lox/Stmt/IfStmt.h"

#include <cassert>

namespace Lox
{
IfStmt::IfStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> thenBranch,
               std::unique_ptr<Stmt> elseBranch) :
    condition(std::move(condition)),
    thenBranch(std::move(thenBranch)),
    elseBranch(std::move(elseBranch))
{
    assert(this->condition != nullptr);
    assert(this->thenBranch != nullptr);
    // elseBranch is optional
}

std::any IfStmt::accept(StmtVisitor<std::any>& visitor) const
{
    return visitor.visitIfStmt(*this);
}

} // end of namespace Lox

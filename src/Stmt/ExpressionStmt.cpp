#include "lox/Stmt/ExpressionStmt.h"

#include <cassert>

namespace Lox
{
ExpressionStmt::ExpressionStmt(std::unique_ptr<Expr> expr) : expr(std::move(expr))
{
    assert(this->expr != nullptr);
}

std::any ExpressionStmt::accept(StmtVisitor<std::any>& visitor) const
{
    return visitor.visitExpressionStmt(*this);
}

} // end of namespace Lox

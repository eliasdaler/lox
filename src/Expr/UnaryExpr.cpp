#include "lox/Expr/UnaryExpr.h"

#include <cassert>

namespace Lox
{
UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> right) : op(op), right(std::move(right))
{
    assert(this->right != nullptr);
}

std::any UnaryExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitUnaryExpr(*this);
}

} // end of namespace Lox

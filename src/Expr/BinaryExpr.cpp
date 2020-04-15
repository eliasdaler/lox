#include "lox/Expr/BinaryExpr.h"

#include <cassert>

namespace Lox
{
BinaryExpr::BinaryExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
    left(std::move(left)),
    op(op),
    right(std::move(right))
{
    assert(this->left != nullptr);
    assert(this->right != nullptr);
}

std::any BinaryExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitBinaryExpr(*this);
}

} // end of namespace Lox

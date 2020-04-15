#include "lox/Expr/LogicalExpr.h"

#include <cassert>

namespace Lox
{
LogicalExpr::LogicalExpr(std::unique_ptr<Expr> left, Token op, std::unique_ptr<Expr> right) :
    left(std::move(left)), op(op), right(std::move(right))
{
    assert(this->left != nullptr);
    assert(this->right != nullptr);
    assert(op.getType() == TokenType::Or || op.getType() == TokenType::And);
}

std::any LogicalExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitLogicalExpr(*this);
}

} // end of namespace Lox

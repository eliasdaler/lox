#include "lox/BinaryExpr.h"

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

} // end of namespace Lox

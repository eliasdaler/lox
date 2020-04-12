#include "lox/UnaryExpr.h"

#include <cassert>

namespace Lox
{
UnaryExpr::UnaryExpr(Token op, std::unique_ptr<Expr> right) : op(op), right(std::move(right))
{
    assert(this->right != nullptr);
}

} // end of namespace Lox

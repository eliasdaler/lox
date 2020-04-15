#include "lox/Expr/GroupingExpr.h"

#include <cassert>

namespace Lox
{
GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr) : expr(std::move(expr))
{
    assert(this->expr != nullptr);
}

std::any GroupingExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitGroupingExpr(*this);
}

} // end of namespace Lox

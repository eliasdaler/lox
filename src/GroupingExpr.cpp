#include "lox/GroupingExpr.h"

#include <cassert>

namespace Lox
{
GroupingExpr::GroupingExpr(std::unique_ptr<Expr> expr) : expr(std::move(expr))
{
    assert(this->expr != nullptr);
}

} // end of namespace Lox

#include "lox/Expr/AssignExpr.h"

#include <cassert>

namespace Lox
{
AssignExpr::AssignExpr(Token name, std::unique_ptr<Expr> value) :
    name(std::move(name)), value(std::move(value))
{
    assert(this->value != nullptr);
}

std::any AssignExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitAssignExpr(*this);
}

} // end of namespace Lox

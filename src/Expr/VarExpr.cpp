#include "lox/Expr/VarExpr.h"

namespace Lox
{
VarExpr::VarExpr(Token name) : name(std::move(name))
{}

std::any VarExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitVarExpr(*this);
}

} // end of namespace Lox

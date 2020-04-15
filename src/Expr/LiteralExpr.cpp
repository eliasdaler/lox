#include "lox/Expr/LiteralExpr.h"

namespace Lox
{
LiteralExpr::LiteralExpr(std::any literal) : literal(std::move(literal))
{}

std::any LiteralExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitLiteralExpr(*this);
}

} // end of namespace Lox

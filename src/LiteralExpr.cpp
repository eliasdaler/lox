#include "lox/LiteralExpr.h"

namespace Lox
{
LiteralExpr::LiteralExpr(std::any literal) : literal(std::move(literal))
{}

} // end of namespace Lox

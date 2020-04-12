#pragma once

#include <any>

#include "Expr.h"
#include "Token.h"

namespace Lox
{
class LiteralExpr : public Expr {
public:
    LiteralExpr(std::any literal);

private:
    std::any literal;
};
} // end of namespace Lox

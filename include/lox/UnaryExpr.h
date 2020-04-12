#pragma once

#include <memory>

#include "Expr.h"
#include "Token.h"

namespace Lox
{
class UnaryExpr : public Expr {
public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right);

private:
    Token op;
    std::unique_ptr<Expr> right;
};
} // end of namespace Lox

#pragma once

#include <memory>

#include "Expr.h"
#include "Token.h"

namespace Lox
{
class BinaryExpr : public Expr {
public:
    BinaryExpr(std::unique_ptr<Expr>left, Token op, std::unique_ptr<Expr> right);

private:
    std::unique_ptr<Expr> left;
    Token op;
    std::unique_ptr<Expr> right;
};
} // end of namespace Lox

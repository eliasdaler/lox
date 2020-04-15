#pragma once

#include <memory>

#include "Expr.h"
#include "ExprVisitor.h"

#include "lox/Token.h"

namespace Lox
{
class UnaryExpr : public Expr {
public:
    UnaryExpr(Token op, std::unique_ptr<Expr> right);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Token& getOp() const { return op; }
    const Expr& getRightExpr() const { return *right; }

private:
    Token op;
    std::unique_ptr<Expr> right;
};

} // end of namespace Lox

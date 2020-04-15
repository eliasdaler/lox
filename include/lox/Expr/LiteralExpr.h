#pragma once

#include <any>

#include "Expr.h"
#include "ExprVisitor.h"

namespace Lox
{
class LiteralExpr : public Expr {
public:
    LiteralExpr(std::any literal);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const std::any& getLiteral() const { return literal; }

private:
    std::any literal;
};

} // end of namespace Lox

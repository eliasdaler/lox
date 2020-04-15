#pragma once

#include <any>

#include "Expr.h"
#include "ExprVisitor.h"

#include "lox/Token.h"

namespace Lox
{
class VarExpr : public Expr {
public:
    VarExpr(Token name);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Token& getName() const { return name; }
private:
    Token name;
};

} // end of namespace Lox

#pragma once

#include <any>
#include <memory>

#include "Expr.h"
#include "ExprVisitor.h"

#include "lox/Token.h"

namespace Lox
{
class AssignExpr : public Expr {
public:
    AssignExpr(Token name, std::unique_ptr<Expr> value);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Token& getName() const { return name; }
    const Expr& getValue() const { return *value; }

private:
    Token name;
    std::unique_ptr<Expr> value;
};

} // end of namespace Lox

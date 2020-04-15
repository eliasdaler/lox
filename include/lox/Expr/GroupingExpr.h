#pragma once

#include <memory>

#include "Expr.h"
#include "ExprVisitor.h"

namespace Lox
{
class GroupingExpr : public Expr {
public:
    GroupingExpr(std::unique_ptr<Expr> expr);
    std::any accept(ExprVisitor<std::any>& visitor) const override;

    const Expr& getExpr() const { return *expr; }

private:
    std::unique_ptr<Expr> expr;
};

} // end of namespace Lox

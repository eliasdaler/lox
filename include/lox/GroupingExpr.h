#pragma once

#include <memory>

#include "Expr.h"
#include "Token.h"

namespace Lox
{
class GroupingExpr : public Expr {
public:
    GroupingExpr(std::unique_ptr<Expr> expr);

private:
    std::unique_ptr<Expr> expr;
};
} // end of namespace Lox

#pragma once

#include <memory>

#include "StmtVisitor.h"
#include "Stmt.h"

#include "lox/Expr/Expr.h"

namespace Lox
{
class ExpressionStmt : public Stmt {
public:
    ExpressionStmt(std::unique_ptr<Expr> expr);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    const Expr& getExpr() const { return *expr; }

private:
    std::unique_ptr<Expr> expr;
};

} // end of namespace Lox

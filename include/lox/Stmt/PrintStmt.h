#pragma once

#include <memory>

#include "Stmt.h"
#include "StmtVisitor.h"

#include "lox/Expr/Expr.h"

namespace Lox
{
class PrintStmt : public Stmt {
public:
    PrintStmt(std::unique_ptr<Expr> expr);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    const Expr& getExpr() const { return *expr; }

private:
    std::unique_ptr<Expr> expr;
};

} // end of namespace Lox

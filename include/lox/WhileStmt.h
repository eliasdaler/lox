#pragma once

#include <memory>

#include "Expr.h"
#include "Stmt.h"
#include "StmtVisitor.h"

namespace Lox
{
class WhileStmt : public Stmt {
public:
    WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    const Expr& getCondition() const { return *condition; }
    const Stmt& getBody() const { return *body; }

private:
    std::unique_ptr<Expr> condition;
    std::unique_ptr<Stmt> body;
};

} // end of namespace Lox

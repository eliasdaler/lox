#pragma once

#include <memory>

#include "Stmt.h"
#include "StmtVisitor.h"

#include "lox/Expr/Expr.h"
#include "lox/Token.h"

namespace Lox
{
class VarStmt : public Stmt {
public:
    VarStmt(Token name, std::unique_ptr<Expr> expr);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    const Token& getName() const { return name; }

    bool hasInitializer() const { return expr != nullptr; }
    const Expr& getInitializer() const { return *expr; }

private:
    Token name;
    std::unique_ptr<Expr> expr;
};

} // end of namespace Lox

#pragma once

#include <memory>

#include "Stmt.h"
#include "StmtVisitor.h"

#include "lox/Expr/Expr.h"
#include "lox/Token.h"

namespace Lox
{
class ReturnStmt : public Stmt {
public:
    ReturnStmt(Token keyword, std::unique_ptr<Expr> value);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    bool hasValue() const { return value.get() != nullptr; }
    const Expr& getValue() const { return *value; }

private:
    Token keyword;
    std::unique_ptr<Expr> value;
};

} // end of namespace Lox


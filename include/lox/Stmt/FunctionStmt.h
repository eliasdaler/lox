#pragma once

#include <memory>
#include <vector>

#include "Stmt.h"
#include "StmtVisitor.h"

#include "lox/Expr/Expr.h"
#include "lox/Token.h"

namespace Lox
{
class FunctionStmt : public Stmt {
public:
    FunctionStmt(Token name, std::vector<Token> params, std::vector<std::unique_ptr<Stmt>> body);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    const Token& getName() const;
    const std::vector<Token>& getParams() const;
    const std::vector<std::unique_ptr<Stmt>>& getBody() const;

private:
    Token name;
    std::vector<Token> params;
    std::vector<std::unique_ptr<Stmt>> body;
};

} // end of namespace Lox


#include "lox/Stmt/FunctionStmt.h"

#include <cassert>

namespace Lox
{
FunctionStmt::FunctionStmt(Token name, std::vector<Token> params,
                           std::vector<std::unique_ptr<Stmt>> body) :
    name(name), params(std::move(params)), body(std::move(body))
{
    assert(name.getType() == TokenType::Identifier);
}

const Token& FunctionStmt::getName() const
{
    return name;
}

const std::vector<Token>& FunctionStmt::getParams() const
{
    return params;
}

const std::vector<std::unique_ptr<Stmt>>& FunctionStmt::getBody() const
{
    return body;
}

std::any FunctionStmt::accept(StmtVisitor<std::any>& visitor) const
{
    return visitor.visitFunctionStmt(*this);
}

} // end of namespace Lox


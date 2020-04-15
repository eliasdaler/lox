#include "lox/Stmt/VarStmt.h"

#include <cassert>

namespace Lox
{
VarStmt::VarStmt(Token name, std::unique_ptr<Expr> expr) :
    name(std::move(name)), expr(std::move(expr))
{
    assert(this->name.getType() == TokenType::Identifier);
}

std::any VarStmt::accept(StmtVisitor<std::any>& visitor) const
{
    return visitor.visitVarStmt(*this);
}

} // end of namespace Lox

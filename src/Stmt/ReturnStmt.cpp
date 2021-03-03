#include "lox/Stmt/ReturnStmt.h"

#include <cassert>

namespace Lox
{
ReturnStmt::ReturnStmt(Token keyword, std::unique_ptr<Expr> value) :
    keyword(std::move(keyword)), value(std::move(value))
{
    assert(this->keyword.getType() == TokenType::Return);
}

std::any ReturnStmt::accept(StmtVisitor<std::any>& visitor) const
{
    return visitor.visitReturnStmt(*this);
}

} // end of namespace Lox


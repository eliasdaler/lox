#include "lox/Stmt/WhileStmt.h"

#include <cassert>

namespace Lox
{
WhileStmt::WhileStmt(std::unique_ptr<Expr> condition, std::unique_ptr<Stmt> body) :
    condition(std::move(condition)), body(std::move(body))
{
    assert(this->condition != nullptr);
    assert(this->body != nullptr);
}

std::any WhileStmt::accept(StmtVisitor<std::any>& visitor) const
{
    return visitor.visitWhileStmt(*this);
}

} // end of namespace Lox

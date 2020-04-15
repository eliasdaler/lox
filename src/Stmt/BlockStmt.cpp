#include "lox/Stmt/BlockStmt.h"

#include <cassert>

namespace Lox
{
BlockStmt::BlockStmt(std::vector<std::unique_ptr<Stmt>> statements) :
    statements(std::move(statements))
{}

std::any BlockStmt::accept(StmtVisitor<std::any>& visitor) const
{
    return visitor.visitBlockStmt(*this);
}

} // end of namespace Lox

#pragma once

#include <memory>
#include <vector>

#include "Expr.h"
#include "Stmt.h"
#include "StmtVisitor.h"

namespace Lox
{
class BlockStmt : public Stmt {
public:
    BlockStmt(std::vector<std::unique_ptr<Stmt>> statements);
    std::any accept(StmtVisitor<std::any>& visitor) const override;

    const std::vector<std::unique_ptr<Stmt>>& getStatements() const { return statements; }

private:
    std::vector<std::unique_ptr<Stmt>> statements;
};

} // end of namespace Lox


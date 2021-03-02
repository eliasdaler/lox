#include "lox/Expr/CallExpr.h"

#include <cassert>

namespace Lox
{
CallExpr::CallExpr(std::unique_ptr<Expr> callee, Token paren,
                   std::vector<std::unique_ptr<Expr>> arguments) :
    callee(std::move(callee)), paren(paren), arguments(std::move(arguments))
{
    assert(paren.getType() == TokenType::RightParen);
}

std::any CallExpr::accept(ExprVisitor<std::any>& visitor) const
{
    return visitor.visitCallExpr(*this);
}

} // end of namespace Lox

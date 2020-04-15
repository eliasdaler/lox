#pragma once

#include "lox/Expr/ExprVisitor.h"

#include <any>

namespace Lox
{
class Expr {
public:
    virtual ~Expr() = default;

    // TODO: figure out a way to accept template visitors? (complicated!)
    virtual std::any accept(ExprVisitor<std::any>& visitor) const = 0;
};
} // end of namespace Lox

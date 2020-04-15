#pragma once

#include "lox/Stmt/StmtVisitor.h"

#include <any>

namespace Lox
{
class Stmt {
public:
    virtual ~Stmt() = default;

    // TODO: figure out a way to accept template visitors? (complicated!)
    virtual std::any accept(StmtVisitor<std::any>& visitor) const = 0;
};
} // end of namespace Lox

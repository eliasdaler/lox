#include "lox/Callable.h"

namespace Lox
{
Callable::Callable(int arity, FuncType f) : arity(arity), f(f)
{}

int Callable::getArity() const
{
    return arity;
}

std::any Callable::call(Interpreter& intepreter, const std::vector<std::any>& arguments) const
{
    return f(intepreter, arguments);
}
} // end of namespace Lox

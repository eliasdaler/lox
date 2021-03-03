#pragma once

#include "Callable.h"

namespace Lox
{
class ClockCallable : public Callable {
public:
    int getArity() override;
    std::any call(Interpreter& intepreter, const std::vector<std::any>& arguments) override;

private:
    int arity;
    std::function<void(Interpreter& intepreter, const std::vector<std::any>& arguments>;
};
} // end of namespace Lox

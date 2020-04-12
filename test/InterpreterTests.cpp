#include <gtest/gtest.h>

#include "lox/Interpreter.h"
#include "lox/Parser.h"
#include "lox/Scanner.h"

using namespace Lox;

TEST(InterpreterTests, Basic)
{
    const auto testScript = R"(42 / 3 + 5 * 20)";
    Scanner scanner{testScript};
    Parser parser{scanner.scanTokens()};
    auto expr = parser.parse();
    ASSERT_NE(nullptr, expr);

    Interpreter interpreter;
    const auto value = interpreter.intepret(*expr);
    ASSERT_EQ(typeid(double), value.type());
    EXPECT_EQ(42. / 3. + 5. * 20., std::any_cast<double>(value));
    // TODO: more tests, of course
}

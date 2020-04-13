#include <gtest/gtest.h>

#include "lox/Interpreter.h"
#include "lox/Parser.h"
#include "lox/Scanner.h"

#include <sstream>

using namespace Lox;

TEST(InterpreterTests, Basic)
{
    const auto testScript = R"(print "hello";)";
    Scanner scanner{testScript};
    Parser parser{scanner.scanTokens()};
    const auto statements = parser.parse();

    ASSERT_EQ(1, statements.size());

    std::stringstream ss;
    Interpreter interpreter(ss);
    interpreter.intepret(statements);

    EXPECT_STREQ("hello\n", ss.str().c_str());
}

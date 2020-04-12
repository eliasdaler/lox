#include <gtest/gtest.h>

#include "lox/BinaryExpr.h"
#include "lox/Parser.h"
#include "lox/Scanner.h"

using namespace Lox;

TEST(ParserTests, Basic)
{
    const auto testScript = R"(2 + 2 == 4)";
    Scanner scanner{testScript};
    Parser parser{scanner.scanTokens()};
    auto expr = parser.parse();

    // TODO: more tests!
    EXPECT_NE(nullptr, expr);
    EXPECT_NE(nullptr, dynamic_cast<BinaryExpr*>(expr.get()));
}

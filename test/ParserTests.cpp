#include <gtest/gtest.h>

#include "lox/Expr/BinaryExpr.h"
#include "lox/Parser.h"
#include "lox/Scanner.h"
#include "lox/Stmt/ExpressionStmt.h"
#include "lox/Stmt/Stmt.h"

using namespace Lox;

TEST(ParserTests, Basic)
{
    const auto testScript = R"(2 + 2 == 4;)";
    Scanner scanner{testScript};
    Parser parser{scanner.scanTokens()};
    auto statements = parser.parse();

    // TODO: more tests!
    ASSERT_EQ(1, statements.size());
    auto* exprStmt = dynamic_cast<ExpressionStmt*>(statements[0].get());
    ASSERT_NE(nullptr, exprStmt);

    auto& expr = exprStmt->getExpr();
    EXPECT_NE(nullptr, dynamic_cast<const BinaryExpr*>(&expr));
}

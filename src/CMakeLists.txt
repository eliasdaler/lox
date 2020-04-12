add_library(lox)

target_include_directories(lox
  PUBLIC
    ${lox_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(lox
  PRIVATE
    Lox.cpp
    Parser.cpp
    Scanner.cpp
    Token.cpp
    # expressions
    BinaryExpr.cpp
    GroupingExpr.cpp
    LiteralExpr.cpp
    UnaryExpr.cpp
)

add_executable(lox_repl)

target_include_directories(lox_repl
  PUBLIC
    ${lox_SOURCE_DIR}/include
  PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_sources(lox_repl
  PRIVATE
    main.cpp
)

target_link_libraries(lox_repl PRIVATE lox)

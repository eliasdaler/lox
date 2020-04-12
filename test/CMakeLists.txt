project(UnitTest
  LANGUAGES CXX
  VERSION 0.1
)

add_executable(unit_test)
add_dependencies(check unit_test)

target_sources(unit_test
  PRIVATE
    InterpreterTests.cpp
    ParserTests.cpp
    ScannerTests.cpp
    # Other files
    main.cpp
)

set(EXECUTABLE_NAME "unit_test")
set_target_properties(unit_test PROPERTIES
  OUTPUT_NAME ${EXECUTABLE_NAME}
)

target_link_libraries(unit_test
  PUBLIC
    gtest
    lox
)

# discover Google Test tests
include(GoogleTest)
gtest_discover_tests(unit_test)

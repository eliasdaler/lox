cmake_minimum_required(VERSION 3.13)

project(lox
  LANGUAGES CXX
  VERSION 0.1
)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)

add_subdirectory(dependencies)
add_subdirectory(src)

if(BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
  add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND} --output-on-failure)
  add_subdirectory(test)
endif()

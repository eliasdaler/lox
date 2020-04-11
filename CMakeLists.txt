cmake_minimum_required(VERSION 3.1)

project(lox
  LANGUAGES CXX
  VERSION 0.1
)

# C++17 is required
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(src)

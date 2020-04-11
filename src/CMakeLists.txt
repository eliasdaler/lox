add_executable(lox)

target_include_directories(lox PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_sources(lox
  PRIVATE
    Lox.cpp
    Token.cpp
    Scanner.cpp
    main.cpp
)

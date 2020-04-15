C++ implementation of Lox
=========================

I'm reading [Crafring Interpreters book](http://craftinginterpreters.com/) by
Bob Nystrom and writing Lox interpreter in C++ as I do this (the original code
in the book is written in Java and some is in C). It's fun!

Chapters complete
---

4. Scanning
6. Parsing Expressions
7. Evaluating Expressions
8. Statements and State
9. Control flow

Building
---

Just run CMake like this:

```sh
# in build dir
cmake <src-dir>
```

and then

```sh
cmake --build . --target lox
```

To build with tests:

```sh
# in build dir
cmake <src-dir> -DBUILD_TESTING=ON
```

To run tests:

```sh
cmake --build . --target check
```

Usage
---

Run a simple example:

```sh
lox examples/test.lox
```

TODO

License
---
This project is licensed under the MIT License, see LICENSE for more information.

Notes
=====

* I use `std::any` instead of Java's `Object`, but could potentially use
  `std::variant<bool, double, std::string>` in the future

* Not sure if string interning will be implemented in the book, but it could
  greatly reduce the cost of copying `std::any` objects - it's cheap when it has
  `bool` or `double` in it, but not so when it has the `std::string` - if there was
  a global table of strings and `std::any` had a pointer to a string, it would
  be much cheaper.

* Visitor pattern as in book doesn't work in C++ - you can't have template
  virtual methods. For now only `ExprVisitor<std::any>` is supported in expressions -
  but that's enough for now.

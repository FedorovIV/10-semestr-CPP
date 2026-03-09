Linker Error Types:

1. Undefined Reference:
   Occurs when a function or variable is declared (e.g., in a header) and used, but its definition (implementation) is not found in any of the object files linked.
   Example: Declaring 'void foo();' in header, calling it in main, but not implementing it in any .cpp file.

2. Multiple Definition:
   Occurs when a function or variable is defined in multiple object files. This often happens if a function definition is placed in a header file without 'inline' keyword, and that header is included in multiple .cpp files.
   Example: 'int x = 5;' in a header included by two source files.

3. Unresolved External Symbol:
   Similar to undefined reference, often used by Windows linkers. Happens when a symbol is referenced but not found.

Demonstration:
To trigger Undefined Reference: Remove Rational::add implementation from rational.cpp and try to build.
To trigger Multiple Definition: Move Rational::add implementation to rational.hpp (without inline) and include rational.hpp in main.cpp and another .cpp file.

For compiling

```
g++ -std=c++20 -Wall -Wextra -Wpedantic -O2 \
    main.cpp rational.cpp \
    -o app

```
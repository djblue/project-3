# Lexer

This project contains a lexer implemented using a DFA. The lexer is
contained in 'include/lexer.h'.

## Building

Building the project is simple. The default make target will build the
project. Just run the following command:

    make

The resulting executable is __run__, which expects two arguments, an input
file name and an output file name.

## Running Tests

Test automation is also provided by way of a make target.

    make test

It will rebuild the project, run the internal unit tests, and the
remaining test cases in the 'tc' directory.

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

## Grammar Rules

    <program>   := [type] [id] (<global> | <function>)
    <global>    := ("," [id]) ";"
    <function>  := "(" <parameter>* ")" "{" <line>* "}"

    <parameter>  := [type] [id] ([type] [id])*
    <parameters> := <parameter> ("," <parameter>)*

    <statement> := <local> | <assign> | <if> | <while> | <return> | <call>
    <block>     := ("{" <statement>* "}" | <statement>)
    <condition> := "(" <expr> ")"

    <local>  := [type] ([id] | [id] "=" <expr>)+ ";"
    <if>     := "if" <condition> <block> "else" <block>
    <while>  := "while" <condition> <block>
    <return> := "return" (<expr>";" | ";")
-   <assign> := [id] "=" <expr> ";"
-   <call>   := [id] "(" <parameters> ")" ";"

    <expr> := <or>
    <or>   := <and> | <and> "||" <and>
    <and>  := <not> | <not> "&&" <not>
    <not>  := <rel> | "!" <rel>

    <rel>  := <sum>
    <rel>  := <sum> (">" <sum>)
    <rel>  := <sum> ("<" <sum>)
    <rel>  := <sum> ("!=" <sum>)
    <rel>  := <sum> (">=" <sum>)
    <rel>  := <sum> ("<=" <sum>)
    <rel>  := <sum> ("==" <sum>)

    <sum>  := <prod>
    <sum>  := <prod> ("+" <prod>)+
    <sum>  := <prod> ("-" <prod>)+

    <prod> := <sign>
    <prod> := <sign> ("/" <sign>)+
    <prod> := <sign> ("*" <sign>)+
    <prod> := <sign> ("%" <sign>)+

    <sign> := <term>
    <sign> := "-" <term>

    <term> := "(" <term> ")"
    <term> := [type] | [id] | [keyword]
    <term> := <call>

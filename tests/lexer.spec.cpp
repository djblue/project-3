#include "../include/test.h"
#include "../include/lexer.h"

void test_lexer () {

    lexer l;

    __title("Testing the Splitter");

    /* splitting tests */
    assert(l.split("a%b").size() == 3,
        "it can split around operators.");
    assert(l.split("a b").size() == 2,
        "it can split around white space.");
    assert(l.split("a%b == 0 && a%c == 0").size() == 14,
        "it can split around complex expressions.");
    assert(l.split("\"hello, world\"").size() == 1,
        "it can split strings with white space.");
    assert(l.split("hello \"hello, world\" hello").size() == 3,
        "it can split around strings and other stuff.");
    assert(l.split("\"hello, world\";hello world").size() == 4,
        "it can split around delimiters.");
    assert(l.split("\'\n hello, world\' world").size() == 2,
        "it can split around single quotes.");

    __end();

    __title("Testing the Lexer");

    /* lexer tests */
    assert(l.lex("--").type == lexer::UNDEFINED,
        "-- is UNDEFINED");
    assert(l.lex("").type == lexer::UNDEFINED,
        "is UNDEFINED");
    assert(l.lex("{").type == lexer::DELIMITER,
        "{ is a DELIMITER");
    assert(l.lex("$123").type == lexer::ID,
        "$123 is a ID");
    assert(l.lex("(").type == lexer::DELIMITER,
        "( is a DELIMITER");
    assert(l.lex("*").type == lexer::OPERATOR,
        "* is an OPERATOR");
    assert(l.lex("+").type == lexer::OPERATOR,
        "+ is an OPERATOR");
    assert(l.lex("+").type == lexer::OPERATOR,
        "+ is an OPERATOR");
    assert(l.lex("1234").type == lexer::INTEGER,
        "1234 is is an INTEGER.");
    assert(l.lex("0").type == lexer::INTEGER,
        "0 is is an INTEGER.");
    assert(l.lex("3.14").type == lexer::FLOAT,
        "3.14 is a FLOAT.");
    assert(l.lex(".3.14").type == lexer::UNDEFINED,
        ".3.14 is not a FLOAT.");
    assert(l.lex("0xAB").type == lexer::HEXADECIMAL,
        "0xAB is a HEXADECIMAL");
    assert(l.lex("0x1234567890ABCD").type == lexer::HEXADECIMAL,
        "0xAB is a large HEXADECIMAL");
    assert(l.lex("015").type == lexer::OCTAL,
        "015 is an OCTAL");
    assert(l.lex("\"hello\"").type == lexer::STRING,
        "\"hello\" is a STRING");
    assert(l.lex("\"$3.45asfa--b123\"").type == lexer::STRING,
        "\"$3.45asfa--b123\" is a STRING");
    assert(l.lex("\'h\'").type == lexer::CHAR,
        "\'h\' is a CHAR");
    assert(l.lex("\'1\'").type == lexer::CHAR,
        "\'1\' is a CHAR");
    assert(l.lex("\'.\'").type == lexer::CHAR,
        "\'.\' is a CHAR");
    assert(l.lex("\'\\n\'").type == lexer::CHAR,
        "escaped CHAR");
    assert(l.lex("\"hello").type == lexer::UNDEFINED,
        "is a broken string working.");
    assert(l.lex("true").type == lexer::BOOL,
        "is a BOOL.");

    __end();
}

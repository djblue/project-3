#include "../include/lexer.h"
#include "../include/test.h"

void test_lexer () {

    lexer l;

    title("Testing the Splitter");

    /* splitting tests */
    assert(l.split("\"hello, world\"").size() == 1,
        "it can split strings with white space.");
    assert(l.split("hello \"hello, world\" hello").size() == 3,
        "it can split around strings and other stuff.");
    assert(l.split("\"hello, world\";hello world").size() == 4,
        "it can split around delimiters.");
    assert(l.split("\'\n hello, world\' world").size() == 2,
        "it can split around single quotes.");

    end();

    title("Testing the Lexer");

    /* lexer tests */
    assert(l.lex("--") == UNDEFINED,
        "-- is UNDEFINED");
    assert(l.lex("") == UNDEFINED,
        "is UNDEFINED");
    assert(l.lex("{") == DELIMITER,
        "{ is a DELIMITER");
    assert(l.lex("$123") == ID,
        "$123 is a ID");
    assert(l.lex("(") == DELIMITER,
        "( is a DELIMITER");
    assert(l.lex("*") == OPERATOR,
        "* is an OPERATOR");
    assert(l.lex("+") == OPERATOR,
        "+ is an OPERATOR");
    assert(l.lex("+") == OPERATOR,
        "+ is an OPERATOR");
    assert(l.lex("1234") == INTEGER,
        "1234 is is an INTEGER.");
    assert(l.lex("0") == ZERO,
        "0 is is an INTEGER.");
    assert(l.lex("3.14") == FLOAT,
        "3.14 is a FLOAT.");
    assert(l.lex(".3.14") == UNDEFINED,
        ".3.14 is not a FLOAT.");
    assert(l.lex("0xAB") == HEXADECIMAL,
        "0xAB is a HEXADECIMAL");
    assert(l.lex("0x1234567890ABCD") == HEXADECIMAL,
        "0xAB is a large HEXADECIMAL");
    assert(l.lex("015") == OCTAL,
        "015 is an OCTAL");
    assert(l.lex("\"hello\"") == STRING,
        "\"hello\" is a STRING");
    assert(l.lex("\"$3.45asfa--b123\"") == STRING,
        "\"$3.45asfa--b123\" is a STRING");
    assert(l.lex("\'h\'") == CHAR,
        "\'h\' is a CHAR");
    assert(l.lex("\'1\'") == CHAR,
        "\'1\' is a CHAR");
    assert(l.lex("\'.\'") == CHAR,
        "\'.\' is a CHAR");
    assert(l.lex("\'\\n\'") == CHAR,
        "escaped CHAR");
    assert(l.lex("\"hello") == UNDEFINED,
        "is a broken string working.");

    end();

}

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
    assert(l.lex("--").type == UNDEFINED,
        "-- is UNDEFINED");
    assert(l.lex("").type == UNDEFINED,
        "is UNDEFINED");
    assert(l.lex("{").type == DELIMITER,
        "{ is a DELIMITER");
    assert(l.lex("$123").type == ID,
        "$123 is a ID");
    assert(l.lex("(").type == DELIMITER,
        "( is a DELIMITER");
    assert(l.lex("*").type == OPERATOR,
        "* is an OPERATOR");
    assert(l.lex("+").type == OPERATOR,
        "+ is an OPERATOR");
    assert(l.lex("+").type == OPERATOR,
        "+ is an OPERATOR");
    assert(l.lex("1234").type == INTEGER,
        "1234 is is an INTEGER.");
    assert(l.lex("0").type == INTEGER,
        "0 is is an INTEGER.");
    assert(l.lex("3.14").type == FLOAT,
        "3.14 is a FLOAT.");
    assert(l.lex(".3.14").type == UNDEFINED,
        ".3.14 is not a FLOAT.");
    assert(l.lex("0xAB").type == HEXADECIMAL,
        "0xAB is a HEXADECIMAL");
    assert(l.lex("0x1234567890ABCD").type == HEXADECIMAL,
        "0xAB is a large HEXADECIMAL");
    assert(l.lex("015").type == OCTAL,
        "015 is an OCTAL");
    assert(l.lex("\"hello\"").type == STRING,
        "\"hello\" is a STRING");
    assert(l.lex("\"$3.45asfa--b123\"").type == STRING,
        "\"$3.45asfa--b123\" is a STRING");
    assert(l.lex("\'h\'").type == CHAR,
        "\'h\' is a CHAR");
    assert(l.lex("\'1\'").type == CHAR,
        "\'1\' is a CHAR");
    assert(l.lex("\'.\'").type == CHAR,
        "\'.\' is a CHAR");
    assert(l.lex("\'\\n\'").type == CHAR,
        "escaped CHAR");
    assert(l.lex("\"hello").type == UNDEFINED,
        "is a broken string working.");

    end();
}

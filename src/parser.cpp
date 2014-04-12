#include "../include/parser.h"

bool parser::expect(string str, string recover) {
    if (peek().text != str) {
        report(peek(), str);
        error_recovery(recover);
        return false;
    } else {
        shift();
        return true;
    }
}

bool parser::expect(lexer::types t, string recover) {
    if (peek().type != t) {
        report(peek(), lexer::type_names[t]);
        error_recovery(recover);
        return false;
    } else {
        shift();
        return true;
    }
}

void parser::report(lexer::token recieved, string expected) {

    struct error e;
    e.expected = expected;
    e.recieved = recieved;

    errors.push_back(e);

}

void parser::print(ostream& out) {
    
    for (it = errors.begin(); it != errors.end(); it++) {
        out << "Line " 
            << (*it).recieved.line 
            << ": expected "
            << (*it).expected 
            << " received \'" 
            << (*it).recieved.text 
            << "\'"<< endl;
    }

}

void parser::error_recovery(string end) {
    int line = peek().line;
    while (tokens.size() > current_token) {
        if (peek().text == end){
            break;
        } else if (line < peek().line) {
            break; 
        } else {
            shift();
        }
    }
}

parser::parser (vector<lexer::token> tokens) {
    status = true;
    current_token = 0;
    line_number = 0;
    this->tokens = tokens;
}
lexer::token parser::shift () {
    lexer::token t = peek();
    current_token++;
    return t;
}
lexer::token parser::peek () {
    if (current_token < tokens.size()) {
        lexer::token t = tokens[current_token];
        line_number = t.line;
        return t;
    } else {
        lexer::token t;
        t.line = line_number;
        t.type = lexer::UNDEFINED;
        t.text = "";
        //cout << "(" << tokens.size() << ","<< current_token << ")" << endl;
        return t;
    }
}
void parser::unshift () {
    current_token--;
}
bool parser::parse () { return program(); }
bool parser::program () {

    current_scope = "global"; // set default scope as global

    if (tokens.size() == 0) {
        return true;
    }
    
    while (current_token < tokens.size()) {
        type(lexer::KEYWORD);
        type(lexer::ID);
        if (peek().text == "(") {
            function();
        } else {
            global();
        }
    }

    return errors.size() == 0;
}
bool parser::global () {
    while (shift().text == ",") {
        sm.table.insert(current_id, current_keyword, current_scope);
        type(lexer::ID);
    }
    sm.table.insert(current_id, current_keyword, current_scope);
    unshift();
    if (peek().text != ";")  {
        report(peek(), ";");
        error_recovery(";");
        return false;
    }
    shift();
    return true;
}
bool parser::function () {
    bool status = true;
    text("(");
    if (!parameters()) {
        error_recovery(")");
        status = false;
    }
    text(")");
    status = status && block();
    return status;
}
bool parser::block () {
    text("{");
    while(peek().text != "}") {
        if (peek().text == "") break;
        if (!line()) {
            return false;
        }
    }
    text("}");
    return true;
}
bool parser::parameters () {
    do {
        if (peek().text != ")") {
            type(lexer::KEYWORD);
            type(lexer::ID);
        }
    } while (shift().text == ",");
    unshift();
    return true;
}
bool parser::line () {

    lexer::token next = shift();
    
    if (next.text == "if") {
        unshift();
        if (_if()) return true;
    }

    else if (next.text == "while") {
        unshift();
        if (_while()) return true;
    }

    else if (next.text == "return") {
        unshift();
        if (_return()) return true;
    }

    else if (next.type == lexer::KEYWORD) {
        unshift();
        if (local()) return true;
    }

    else if (next.type == lexer::ID) {
        lexer::token t = shift();
        
        if (t.text == "=") {
            if (expression() && shift().text == ";") return true;
        } else {
            unshift();
            unshift();
            if (call()) {
                text(";");
                return true;
            }
        }
    }

    unshift();

    error_recovery(";");

    return false;
}
bool parser::local () {
    type(lexer::KEYWORD);
    do {
        type(lexer::ID);
        if (peek().text == "=") {
            shift();
            if(!expression()) return false;
        }
    } while (shift().text == ",");
    unshift();
    text(";");
    return true;
}
bool parser::assign () {
    type(lexer::ID);
    text("=");
    if(!expression()) return false;
    text(";");
    return true;
}
bool parser::_if () {
    
    bool status = true;
    text("if");

    text("(");
    if (!expression()) {
        error_recovery(")");
        status = false;
    }
    text(")");

    sm.coditions(line_number);

    if (peek().text == "{") {
        if (!block()) return false;
    } else {
        line();
    }

    if (peek().text == "else") {
        text("else");
        if (peek().text == "{") {
            if (!block()) return false;
        } else {
            line();
        }
    }

    return status;
}
bool parser::_while () {

    bool status = true;;

    text("while");

    text("(");
    if (!expression()) {
        error_recovery(")");
        status = false;
    }
    text(")");

    sm.coditions(line_number);

    if (peek().text == ";") {
        return true;
    } else if (peek().text == "{") {
        if (!block()) return false;
    } else {
        if (!line()) return false;
    }

    return status;
}
bool parser::_return () {
    text("return");
    if (!expression()) return false;
    text(";");   
    return true;
}
bool parser::call() {
    type(lexer::ID);
    text ("(");

    // not an empty function call
    if (peek().text != ")")  {
        do {
            if (!expression()) {
                error_recovery(")");
                shift();
                return false;
            }
        } while (shift().text == ",");
        unshift();
    }

    text(")");
    return true;
}
bool parser::expression () {
    bool once = false;
    do {
        if (once) sm.calculatTypeBinary("|");
        if (!_and()) return false; 
        once = true;
    } while (shift().text == "|");
    unshift();
    return true;
}
bool parser::_and () {
    bool once = false;
    do {
        if (once) sm.calculatTypeBinary("&");
        if (!_not()) return false; 
        once = true; 
    } while (shift().text == "&");
    unshift();
    return true;
}
bool parser::_not () {
    bool status = false;
    if (shift().text != "!") {
        unshift();
    } else {
        status = true;
    }
    if (!relational()) return false;
    if (status) sm.calculatTypeUnary("!");
    return true;
}
bool parser::relational () {

    string op = "";

    do {

        if (!sum()) return false;

        if (op != "") {
            sm.calculatTypeBinary(op);
        }

        if (shift().text == "<") {
            op = "<";
            continue;
        }
        unshift();
        if (shift().text == ">") {
            op = ">";
            continue;
        }
        unshift();

        break;

    } while (true);

    return true;
}
bool parser::sum () {

    string op = "";

    do {

        if (!product()) return false;

        if (op != "") {
            sm.calculatTypeBinary(op);
        }

        if (shift().text == "+") {
            op = "+";
            continue;
        }
        unshift();
        if (shift().text == "-") {
            op = "-";
            continue;
        }
        unshift();

        break;

    } while (true);

    return true;
}
bool parser::product () {
    string op = "";
    do {
        if (op != "") {
            sm.calculatTypeBinary(op);
        }

        if (!sign()) return false;

        if (shift().text == "*") {
            op = "*";
            continue;
        }
        unshift();
        if (shift().text == "/") {
            op = "/";
            continue;
        }
        unshift();
        if (shift().text == "%") {
            op = "%";
            continue;
        }
        unshift();

        break;

    } while (true);

    return true;
}
bool parser::sign () {
    if (shift().text == "-") {
        if (!terminal()) {
            return false;
        } else {
            sm.calculatTypeUnary("-");
        }
    } else {
        unshift();
        if (!terminal()) return false;
    }
    return true;
}
bool parser::terminal () {
    if (shift().type == lexer::ID) {
        if (peek().text == "(") {
            unshift(); 
            return call();
        }
    }

    unshift();
    
    if (shift().text == "(") {
        if (!expression()) return false;
        text(")");
    } else {
        unshift();
        lexer::token t = peek();
        if (t.type < lexer::INTEGER || t.type > lexer::BOOL) { 
            report (t, string("value or identifier"));
            return false;
        } else {
            sm.type_stack.push_back(lexer::type_names[t.type]);
            shift();
        }
    }
    return true;
}

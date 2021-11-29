/*
 * File: statement.cpp
 * -------------------
 * This file implements the constructor and destructor for
 * the Statement class itself.  Your implementation must do
 * the same for the subclasses you define for each of the
 * BASIC statements.
 */

#include <string>
#include "statement.h"

using namespace std;

/* Implementation of the Statement class */

Statement::Statement() {
    /* Empty */
}

Statement::~Statement() {
    /* Empty */
}

if_statement::if_statement() {

}

if_statement::if_statement(string line) {
    raw_line = line;
}

void if_statement::execute(EvalState &state) {
    get_pos();
    get_value(state);
}

bool if_statement::if_find(Program program) {
    if (program.source_line.find(object_num) != program.source_line.end())
        return true;
    else
        return false;
}

bool if_statement::if_operate() {
    if (raw_line[op_pos] == '=' && l_value == r_value)
        return true;
    if (raw_line[op_pos] == '<' && l_value < r_value)
        return true;
    if (raw_line[op_pos] == '>' && l_value > r_value)
        return true;
    return false;
}

void if_statement::get_pos() {
    while (raw_line[op_pos] != '=' && raw_line[op_pos] != '<' && raw_line[op_pos] != '>')
        op_pos++;
    while (raw_line.substr(if_pos, 2) != "IF")
        if_pos++;
    while (raw_line.substr(r_end, 4) != "THEN")
        r_end++;
    l = raw_line.substr(if_pos + 2, op_pos - 2 - if_pos);
    r = raw_line.substr(op_pos + 1, r_end - op_pos - 1);
}

void if_statement::get_value(EvalState state) {

    scanner_temp.ignoreWhitespace();
    scanner_temp.setInput(l);
    Expression *exp_temp = parseExp(scanner_temp);
    l_value = exp_temp->eval(state);
    scanner_temp.ignoreWhitespace();
    scanner_temp.setInput(r);
    exp_temp = parseExp(scanner_temp);
    r_value = exp_temp->eval(state);
    scanner_temp.setInput(raw_line);
    string trush = scanner_temp.nextToken();
    while (trush != "THEN")
        trush = scanner_temp.nextToken();
    Expression *exp = parseExp(scanner_temp);
    object_num = exp->eval(state);
}

if_statement::~if_statement() {

}
goto_statement::goto_statement(){

}
goto_statement::goto_statement(string line) {
    scanner_temp.ignoreWhitespace();
    scanner_temp.setInput(line);
}

goto_statement::~goto_statement(){

}

void goto_statement::execute(EvalState &state){
    string trush=scanner_temp.nextToken();
    trush=scanner_temp.nextToken();
    Expression *exp = parseExp(scanner_temp);
    object_num = exp->eval(state);
}
bool goto_statement::if_find(Program program){
    if (program.source_line.find(object_num) != program.source_line.end())
        return true;
    else
        return false;
}

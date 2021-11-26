/*
 * File: Basic.cpp
 * ---------------
 * This file is the starter project for the BASIC interpreter.
 */

#include <cctype>
#include <iostream>
#include <string>
#include "exp.h"
#include "parser.h"
#include "program.h"
#include "../StanfordCPPLib/error.h"
#include "../StanfordCPPLib/tokenscanner.h"

#include "../StanfordCPPLib/simpio.h"
#include "../StanfordCPPLib/strlib.h"

#include <stdlib.h>


using namespace std;

/* Function prototypes */

void processLine(string line, Program &program, EvalState &state);

bool ifreserved(string);

void run_program(Program);

bool ifinterger(string);

/* Main program */
bool ifsingle;
int line_num;
string sta;
int run_line_num;
bool ifrun = false;
int ifend = false;

class error_out {
public:
    error_out() {
        ifrun = true;
    }

    ~error_out() {
        ifrun = false;
    }
};

int main() {
    EvalState state;
    Program program;
//    cout << "Stub implementation of BASIC" << endl;
    while (true) {
        try {
            string input = getLine();
            if (input.empty())
                return 0;
            processLine(input, program, state);
        } catch (ErrorException &ex) {
            cerr << "Error: " << ex.getMessage() << endl;
        }
    }
    return 0;
}

/*
 * Function: processLine
 * Usage: processLine(line, program, state);
 * -----------------------------------------
 * Processes a single line entered by the user.  In this version of
 * implementation, the program reads a line, parses it as an expression,
 * and then prints the result.  In your implementation, you will
 * need to replace this method with one that can respond correctly
 * when the user enters a program line (which begins with a number)
 * or one of the BASIC commands, such as LIST or RUN.
 */

void processLine(string line, Program &program, EvalState &state) {
    if (line[0] >= '0' && line[0] <= '9')
        ifsingle = false;
    else
        ifsingle = true;

    TokenScanner scanner;
    scanner.ignoreWhitespace();
    scanner.scanNumbers();
    scanner.setInput(line);
    if (ifsingle) {
        sta = scanner.nextToken();

        if (sta == "REM") {
        } else if (sta == "QUIT")
            exit(0);
        else if (sta == "HELP")
            cout << "Sorry, I can not help you." << endl;
        else if (sta == "CLEAR") {
            program.clear();
            state.clear();
        } else if (sta == "LIST")
            program.list_program();
        else if (sta == "LET") {
            string l = scanner.nextToken();
            if (ifreserved(l)) {
                cout << "SYNTAX ERROR" << endl;
            } else {
                string trush = scanner.nextToken();
                Expression *exp = parseExp(scanner);
                int value = exp->eval(state);
                state.setValue(l, value);
            }
        } else if (sta == "PRINT") {
            Expression *exp = parseExp(scanner);
            int value = exp->eval(state);
            cout << value << endl;
        } else if (sta == "INPUT") {
            string l = scanner.nextToken();
            cout << " ? ";
            string r;
            getline(cin, r);
            while (!ifinterger(r)) {
                cout << "INVALID NUMBER" << endl;
                cout << " ? ";
                getline(cin, r);
            }
            int value = stoi(r);
            state.setValue(l, value);
        } else if (sta == "RUN" && !ifrun) {
            ifend = false;
            run_line_num = program.getFirstLineNumber();
            error_out o;
            while (!program.iflastline(run_line_num) && !ifend) {
                int run_line_num_pre = run_line_num;
                processLine(program.getSourceLine(run_line_num), program, state);
                if (!program.iflastline(run_line_num) && run_line_num_pre == run_line_num)
                    run_line_num = program.getNextLineNumber(run_line_num);
            }
            if (!ifend)
                processLine(program.getSourceLine(run_line_num), program, state);
        }
    } else {
        string f = scanner.nextToken();
        line_num = stoi(f);
        sta = scanner.nextToken();
        if (f == line)
            program.removeSourceLine(line_num);
        else
            program.addSourceLine(line_num, line);
        if (sta == "REM" && ifrun) {
        } else if (sta == "QUIT" && ifrun)
            exit(0);
        else if (sta == "HELP" && ifrun)
            cout << "Sorry, I can not help you." << endl;
        else if (sta == "CLEAR" && ifrun) {
            program.clear();
            state.clear();
        } else if (sta == "LIST" && ifrun)
            program.list_program();
        else if (sta == "LET" && ifrun) {
            string l = scanner.nextToken();
            if (ifreserved(l)) {
                cout << "SYNTAX ERROR" << endl;
            } else {
                string trush = scanner.nextToken();
                Expression *exp = parseExp(scanner);
                int value = exp->eval(state);
                state.setValue(l, value);
            }
        } else if (sta == "PRINT" && ifrun) {
            Expression *exp = parseExp(scanner);
            int value = exp->eval(state);
            cout << value << endl;
        } else if (sta == "INPUT" && ifrun) {
            string l = scanner.nextToken();
            cout << " ? ";
            string r;
            getline(cin, r);
            while (!ifinterger(r)) {
                cout << "INVALID NUMBER" << endl;
                cout << " ? ";
                getline(cin, r);
            }
            int value = stoi(r);
            state.setValue(l, value);
        } else if (sta == "GOTO" && ifrun) {
            Expression *exp = parseExp(scanner);
            int object_num = exp->eval(state);
            if (program.source_line.find(object_num) == program.source_line.end()) {
                cout << "LINE NUMBER ERROR" << endl;
            } else {
                run_line_num = object_num;
            }
        } else if (sta == "IF" && ifrun) {
            string raw_line = line;
            int op_pos = 0;
            while (raw_line[op_pos] != '=' && raw_line[op_pos] != '<' && raw_line[op_pos] != '>') {
                op_pos++;
            }
            int if_pos=0;
            while (raw_line.substr(if_pos,2)!= "IF")
                if_pos++;
            string l = raw_line.substr(if_pos+2, op_pos - 2-if_pos);
            int r_end = op_pos;
            while (raw_line.substr(r_end, 4) != "THEN")
                r_end++;
            string r = raw_line.substr(op_pos + 1, r_end - op_pos - 1);
            TokenScanner scanner_temp;
            scanner_temp.ignoreWhitespace();
            scanner_temp.setInput(l);
            Expression *exp_temp = parseExp(scanner_temp);
            int l_value = exp_temp->eval(state);
            scanner_temp.ignoreWhitespace();
            scanner_temp.setInput(r);
            exp_temp = parseExp(scanner_temp);
            int r_value = exp_temp->eval(state);
            string trush = scanner.nextToken();
            while (trush != "THEN")
                trush = scanner.nextToken();
            Expression *exp = parseExp(scanner);
            int object_num = exp->eval(state);
            if (program.source_line.find(object_num) == program.source_line.end()) {
                cout << "LINE NUMBER ERROR" << endl;
            } else {
                if (raw_line[op_pos] == '=' && l_value == r_value)
                    run_line_num = object_num;
                else if (raw_line[op_pos] == '<' && l_value < r_value)
                    run_line_num = object_num;
                else if (raw_line[op_pos] == '>' && l_value > r_value)
                    run_line_num = object_num;
            }
        } else if (sta == "RUN" && !ifrun) {
            run_line_num = program.getFirstLineNumber();
            ifrun = true;
            while (!program.iflastline(run_line_num) && !ifend) {
                processLine(program.getSourceLine(run_line_num), program, state);
                run_line_num = program.getNextLineNumber(run_line_num);
            }
            if (!ifend)
                processLine(program.getSourceLine(run_line_num), program, state);
            ifrun = false;
        } else if (sta == "END" && ifrun)
            ifend = true;
    }
//    Expression *exp = parseExp(scanner);
//    int value = exp->eval(state);
//    cout << value << endl;
//    delete exp;
}

bool ifreserved(string l) {
    if (l == "REM")
        return true;
    else if (l == "LET")
        return true;
    else if (l == "PRINT")
        return true;
    else if (l == "INPUT")
        return true;
    else if (l == "END")
        return true;
    else
        return false;
}

bool ifinterger(string r) {
    int length = r.length();
    bool ans = true;
    for (int i = 0; i < length; i++) {
        if ((r[i] >= '0' && r[i] <= '9') || r[i] == '-') {
        } else {
            ans = false;
        }
    }
    return ans;
}

void run_program(Program program) {

}
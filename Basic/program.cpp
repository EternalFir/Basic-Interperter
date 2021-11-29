/*
 * File: program.cpp
 * -----------------
 * This file is a stub implementation of the program.h interface
 * in which none of the methods do anything beyond returning a
 * value of the correct type.  Your job is to fill in the bodies
 * of each of these methods with an implementation that satisfies
 * the performance guarantees specified in the assignment.
 */

#include <string>
#include "program.h"
#include "statement.h"

using namespace std;

Program::Program() {
    // Replace this stub with your own code
}

Program::~Program() {
    // Replace this stub with your own code
}

void Program::clear() {
    // Replace this stub with your own code
    source_line.clear();
//    next_line.clear();
//    first_line_number=-1;
//    pre_line_num=-1;
}

bool Program::iflastline(int line_now){
    auto i=source_line.find(line_now);
    i++;
    if(i==source_line.end())
        return true;
    else
        return false;
}

void Program::addSourceLine(int lineNumber, string line) {
    // Replace this stub with your own code
//    next_line[pre_line_num]=lineNumber;
//    pre_line_num=lineNumber;
//    next_line[lineNumber]=-1;
//    if(first_line_number > lineNumber || first_line_number==-1)
//        first_line_number=lineNumber;
    source_line[lineNumber]=line;
}

void Program::removeSourceLine(int lineNumber) {
    // Replace this stub with your own code
    source_line.erase(lineNumber);
}

string Program::getSourceLine(int lineNumber) {
    return source_line[lineNumber];    // Replace this stub with your own code
}

//void Program::setParsedStatement(int lineNumber, Statement *stmt) {
//    // Replace this stub with your own code
//}

//Statement *Program::getParsedStatement(int lineNumber) {
//    return NULL;  // Replace this stub with your own code
//}

int Program::getFirstLineNumber() {
    auto i= this->source_line.begin();
    return i->first;
    // Replace this stub with your own code
}

int Program::getNextLineNumber(int lineNumber) {
    auto i=source_line.find(lineNumber);
    i++;
    return i->first;
    // Replace this stub with your own code
}

void Program::list_program(){
    for(auto i : this->source_line){
        cout<<i.second<<endl;
    }
}
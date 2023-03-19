#pragma once
#include <iostream>
#include <vector>
#include <string>

#include "Lexeme.h"

std::vector<Lexeme> lexemes;
Lexeme current_lexeme;
int index = -1;

void gl(int i = 0) {
    index++;
    if (index > lexemes.size()) {
        throw std::string("extra characters in line " + std::to_string(current_lexeme.num_of_string_));
    }
    else {
        if (i) index = i;
        current_lexeme = lexemes[index];
        if (current_lexeme.type_ == "incorrect symbol") {
            throw std::string("incorrect symbol in line " + std::to_string(current_lexeme.num_of_string_));
        }
    }
}

void Expression(std::string ret = "any") {
    //дописать
}

void Parametres() {
    if (current_lexeme.content_ == ")") return;
    Identifier();
    while (current_lexeme.content_ != ")") {
        if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        Identifier();
    }
    gl();
}

void Cin() {
    gl();
    if (current_lexeme.content_ != ">>") throw std::string(">> was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Identifier();
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void Cout() {
    gl();
    if (current_lexeme.content_ != "<<") throw std::string("<< was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Identifier();
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void If() {
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Expression("bool");
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Block();
    if (current_lexeme.content_ == "else") {
        gl();
        Block();
    }
}

void While() {
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Expression("bool");
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Block();
}

void Crement() {
    if (current_lexeme.content_ == "++" || current_lexeme.content_ == "--") {
        gl();
        return;
    }
    throw std::string("++ or -- was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void For() {
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (!CorrectType(current_lexeme.content_, 0, 0)) throw std::string("incorrect type in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Identifier(0);
    if (current_lexeme.content_ != "=") throw std::string("= was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Expression();
    while (current_lexeme.content_ != ";") {
        if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (!CorrectType(current_lexeme.content_, 0, 0)) throw std::string("incorrect type in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        Identifier();
        if (current_lexeme.content_ != "=") throw std::string("= was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        Expression();
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Expression("bool");
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ == ")") throw std::string("increment was expected in line " + std::to_string(current_lexeme.num_of_string_));
    while (current_lexeme.content_ != ")") {
        if (current_lexeme.type_ == "identidier") {
            Identifier(0);
            Crement();
        }
        else
        {
            Crement();
            Identifier(0);
        }
        if (current_lexeme.content_ != ",") break;
        gl();
    }
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Block();
}

void Local() {
    if (CorrectType(current_lexeme.content_)) {
        VariableDecloration();
        return;
    }
    if (current_lexeme.type_ == "identifier") {
        Assignment();
        return;
    }
    if (current_lexeme.content_ == "cin") {
        Cin();
        return;
    }
    if (current_lexeme.content_ == "cout") {
        Cout();
        return;
    }
    if (current_lexeme.content_ == "if") {
        If();
        return;
    }
    if (current_lexeme.content_ == "while") {
        While();
        return;
    }
    if (current_lexeme.content_ == "for") {
        For();
        return;
    }
    throw std::string("Variable decloration or assignment was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Block() {
    if (current_lexeme.content_ != "{") {
        throw std::string("{ was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    Local();
    if (current_lexeme.content_ != "}") {
        throw std::string("} was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
}

void FunctionDecloration() {
    gl();
    Identifier();
    if (current_lexeme.content_ != "(") {
        throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    Parametres();
    if (current_lexeme.content_ != ")") {
        throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    Block();
}

void VariableDecloration() {
    bool mass = false;
    if (current_lexeme.content_ == "mass") {
        mass = true;
        gl();
        if (current_lexeme.content_ != "<") throw std::string("type od mass ne ykazan was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (!CorrectType(current_lexeme.content_, 0)) throw std::string("incorrect type od mass in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (current_lexeme.content_ != ">") throw std::string("type od mass ne ykazan was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    Identifier();
    if (current_lexeme.content_ == "=") {
        gl();
        Expression();
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

bool CorrectType(std::string str, bool mass = true, bool string = true) {
    return str == "bool" || str == "char" || string && str == "string" ||str == "int" ||
        str == "longlong" || str == "float" || str == "double" || mass && str == "mass";
}

void Identifier(bool bracket = true) {
    if (current_lexeme.type_ == "identifier") {
        gl();
        if (bracket && current_lexeme.content_ == "[") {
            gl();
            if (current_lexeme.type_ != "integer num") throw std::string("integer num was expected in index line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
        }
        return;
    }
    throw std::string("identifier was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Decloration() {
    int index_of_type = index;
    if (current_lexeme.content_ == "void") {
        FunctionDecloration();
        return;
    }
    if (CorrectType(current_lexeme.content_)) {
        gl();
        Identifier();
        if (current_lexeme.content_ == "(") {
            gl(index_of_type);
            FunctionDecloration();
            return;
        }
        if (current_lexeme.content_ == "=" || current_lexeme.content_ == ";") {
            gl(index_of_type);
            VariableDecloration();
            return;
        }
    }
    throw std::string("variable or function decloration was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Assignment() {
    Identifier();
    if (current_lexeme.content_ != "=") {
        throw std::string("assigment was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    Expression();
    if (current_lexeme.content_ != ";") {
        throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
}

void Global() {
    if (current_lexeme.type_ == "offword") {
        Decloration();
        return;
    }
    if (current_lexeme.type_ == "identifier") {
        Assignment();
        return;
    }
    throw std::string("decloration or assignment was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Program() {
    gl();
    Global();
}

void SyntaxAnalyzer(std::vector<Lexeme> lexemes_) {
    swap(lexemes, lexemes_);
    try {
        Program();
        if (index < lexemes.size()) {
            throw std::string("characters are expected to enter");
        }
    }
    catch (std::string str) {
        std::cout << str;
    }
}
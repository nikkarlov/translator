#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include "Lexeme.h"
#include "Semantic.h"

Lexeme c;
int i = 0;
Itv itv;
Itv itv_params;
std::vector<Lexeme> lexemes;

void gl();
void Exp();
void Type(Itv& itv1);
void Block();
void Exp8();
void Global();

bool Type_() {
    if (c.content_ == "int" || c.content_ == "ll" || c.content_ == "float" || c.content_ == "ld" ||
        c.content_ == "string" || c.content_ == "char" || c.content_ == "array") {
        return true;
    }
    return false;
}
bool Prior1() {
    if (c.content_ == "++" || c.content_ == "--") {
        return true;
    }
    if (c.content_ == "[") {
        gl();
        Exp();
        if (c.content_ != "]") {
            throw std::string("expected to get a symbol ] in line " + std::to_string(c.num_of_string_));
        }
        return true;
    }
    if (c.content_ == "(") {
        gl();
        Exp();
        if (c.content_ != ")") {
            throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
        }
        return true;
    }
    return false;
}
bool Prior2() {
    if (c.content_ == "++" || c.content_ == "--" || c.content_ == "+" || c.content_ == "-" ||
        c.content_ == "~" || c.content_ == "!") {
        return true;
    }
    return false;
}
bool Prior3() {
    if (c.content_ == "*" || c.content_ == "/" || c.content_ == "%") {
        return true;
    }
    return false;
}
bool Prior4() {
    if (c.content_ == "+" || c.content_ == "-") {
        return true;
    }
    return false;
}
bool Prior5() {
    if (c.content_ == ">" || c.content_ == "<" || c.content_ == ">=" || c.content_ == "<=") {
        return true;
    }
    return false;
}
bool Prior6() {
    if (c.content_ == "==" || c.content_ == "!=") {
        return true;
    }
    return false;
}
bool Prior7() {
    if (c.content_ == "&&") {
        return true;
    }
    return false;
}
bool Prior8() {
    if (c.content_ == "||") {
        return true;
    }
    return false;
}
bool Prior9() {
    if (c.content_ == "=" || c.content_ == "+=" || c.content_ == "-=" || c.content_ == "*=" ||
        c.content_ == "/=" || c.content_ == "%=") {
        return true;
    }
    return false;
}
void gl() {
    if (i > lexemes.size()) {
        throw std::string("end");
    }
    if (i == lexemes.size()) {
        c.type_ = "end";
    }
    else {
        c = lexemes[i];
        if (c.type_ == "incorrect symbol") {
            throw std::string("incorrect symbol in line " + std::to_string(c.num_of_string_));
        }
    }
    i++;
}
bool String() {
    if (c.content_ == "string") {
        itv.type_ += c.content_;
        return true;
    }
    return false;
}
bool CharType() {
    if (c.content_ == "char") {
        itv.type_ += c.content_;
        return true;
    }
    return false;
}
bool NumType() {
    if (c.content_ == "int" || c.content_ == "ll" || c.content_ == "float" || c.content_ == "ld") {
        itv.type_ += c.content_;
        return true;
    }
    return false;
}
void ArrayType() {
    if (!(NumType() || String() || CharType())) {
        throw std::string("invalid data type in line " + std::to_string(c.num_of_string_));
    }
    gl();
}
bool Array(Itv& itv1) {
    if (c.content_ == "array") {
        itv1.type_ += c.content_;
        gl();
        if (c.content_ != "<") {
            throw std::string("expected to get a symbol < in line " + std::to_string(c.num_of_string_));
        }
        gl();
        ArrayType();
        if (c.content_ != ">") {
            throw std::string("expected to get a symbol > in line " + std::to_string(c.num_of_string_));
        }
        gl();
        if (c.content_ != "[") {
            throw std::string("expected to get a symbol [ in line " + std::to_string(c.num_of_string_));
        }
        gl();
        if (c.content_ != "]") {
            Exp();
        }
        if (c.content_ != "]") {
            throw std::string("expected to get a symbol ] in line " + std::to_string(c.num_of_string_));
        }
        gl();
        return true;
    }
    return false;
}
void Type(Itv& itv1) {
    if (Array(itv1)) return;
    if (!(NumType() || String() || CharType())) {
        throw std::string("invalid data type in line " + std::to_string(c.num_of_string_));
    }
    itv1.type_ += c.content_;
    gl();
}
void FunctionType() {
    if (!(NumType() || String() || CharType() || c.content_ == "void")) {
        throw std::string("invalid data type in line " + std::to_string(c.num_of_string_));
    }
    itv.type_ += c.content_;
    gl();
}
void Parameters() {
    if (c.content_ != ")") {
        Type(itv_params);
        itv_params.identifier_ = c.content_;

        if (c.type_ != "identifier") {
            throw std::string("");
        }
        gl();
    }
    itv.params_.push_back(itv_params);
    itv_params = {};
    while (c.content_ == ",") {
        gl();
        Type(itv_params);
        itv_params.identifier_ = c.content_;
        if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
        }
        gl();
        itv.params_.push_back(itv_params);
        itv_params = {};
    }
}
void Enumeration() {
    if (c.content_ != ")") {
        if (c.type_ == "string literal") {
            gl();
        }
        else {
            Exp();
        }
    }
    while (c.content_ == ",") {
        gl();
        if (c.type_ == "string literal") {
            gl();
        }
        else {
            Exp();
        }
    }
}
void Exp1() {
    if (c.type_ == "numeric ll literal" || c.type_ == "numeric ld literal") {
        gl();
    }
    else if (c.type_ == "identifier") {
        gl();
        if (c.content_ == "(") {
            gl();
            Enumeration();
            if (c.content_ != ")") {
                throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
            }
            gl();
        }
        else if (Prior1()) {
            gl();
            if (Prior1()) {
                gl();
            }
        }
    }
    else if (c.content_ != "(") {
        throw std::string("expected to get a expression in line " + std::to_string(c.num_of_string_));
    }
    else {
        gl();
        Exp8();
        if (c.content_ != ")") {
            throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
        }
        gl();
    }
}
void Exp2() {
    if (Prior2()) {
        gl();
    }
    Exp1();
}
void Exp3() {
    Exp2();
    while (Prior3()) {
        gl();
        Exp2();
    }
}
void Exp4() {
    Exp3();
    while (Prior4()) {
        gl();
        Exp3();
    }
}
void Exp5() {
    Exp4();
    while (Prior5()) {
        gl();
        Exp4();
    }
}
void Exp6() {
    Exp5();
    while (Prior6()) {
        gl();
        Exp5();
    }
}
void Exp7() {
    Exp6();
    while (Prior7()) {
        gl();
        Exp6();
    }
}
void Exp8() {
    Exp7();
    while (Prior8()) {
        gl();
        Exp7();
    }
}
void Exp() {
    Exp8();
}
void Lvalue() {
    if (c.type_ != "identifier") {
        throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
    }
    gl();
    if (c.content_ == "[") {
        gl();
        Exp();
        if (c.content_ != "]") {
            throw std::string("expected to get a symbol ] in line " + std::to_string(c.num_of_string_));
        }
        gl();
    }
}
void Exp9() {
    Lvalue();
    if (!Prior9()) {
        throw std::string("expected assignment in line " + std::to_string(c.num_of_string_));
    }
    if (c.content_ == "=") {
        gl();
        if (c.type_ == "string literal") {
            gl();
        }
        else {
            Exp8();
        }
    }
    else {
        gl();
        Exp8();
    }
}
void While() {
    if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
    }
    gl();
    Exp();
    if (c.content_ != ")") {
        throw std::string("expected to get a symbol ] ) in line " + std::to_string(c.num_of_string_));
    }
    gl();
    current_scope->NewScope();
    Block();
    current_scope->Back();
}

void For() {
    current_scope->NewScope();
    if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
    }
    gl();
    Itv temp_itv;
    if (Type_()) {
        temp_itv.type_ = c.content_;
        itv.type_ = c.content_;
        gl();
        if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
        }
        itv.identifier_ = c.content_;
        if (current_scope->CheckId(itv.identifier_)) {
            throw std::string("variable is already declared in line " + std::to_string(c.num_of_string_));
        }
        current_scope->PushId(itv);
        gl();
        if (c.content_ == "=") {
            gl();
            if (c.type_ == "string literal") {
                gl();
            }
            else {
                Exp();
            }
        }
        while (c.content_ == ",") {
            gl();
            if (c.type_ != "identifier") {
                throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
            }
            itv.type_ = temp_itv.type_;
            itv.identifier_ = c.content_;
            gl();
            if (c.content_ == "=") {
                gl();
                if (c.type_ == "string literal") {
                    gl();
                }
                else {
                    Exp();
                }
            }
            if (current_scope->CheckId(itv.identifier_)) {
                throw std::string("variable is already declared in line " + std::to_string(c.num_of_string_));
            }
            current_scope->PushId(itv);
        }
        if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
        }
    }
    else if (c.type_ == "identifier") {
        if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
        }
        gl();
        if (c.content_ == "=") {
            gl();
            if (c.type_ == "string literal") {
                gl();
            }
            else {
                Exp();
            }
        }
        while (c.content_ == ",") {
            gl();
            if (c.type_ != "identifier") {
                throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
            }
            gl();
            if (c.content_ == "=") {
                gl();
                if (c.type_ == "string literal") {
                    gl();
                }
                else {
                    Exp();
                }
            }
        }
        if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
        }
    }
    if (c.content_ == ";") {
        gl();
    }
    else {
        throw std::string("incorrect cycle definition in line " + std::to_string(c.num_of_string_));
    }
    Exp();
    if (c.content_ != ";") {
        throw std::string(" expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
    }
    gl();
    Exp();
    if (c.content_ != ")") {
        throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
    }
    gl();
    Block();
    current_scope->Back();
}
void If() {
    if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
    }
    gl();
    Exp();
    if (c.content_ != ")") {
        throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
    }
    gl();
    current_scope->NewScope();
    Block();
    current_scope->Back();
    if (c.content_ == "else") {
        gl();
        current_scope->NewScope();
        Block();
        current_scope->Back();
    }
}
void Return() {
    if (c.type_ == "string literal") {
        gl();
    }
    else if (c.content_ != ";") {
        Exp();
    }
}
void Cout() {
    if (c.content_ != "<<") {
        throw std::string("expected to get a symbols << in line " + std::to_string(c.num_of_string_));
    }
    gl();
    if (c.type_ == "string literal") {
        gl();
    }
    else {
        Exp();
    }
    while (c.content_ == "<<") {
        gl();
        if (c.type_ == "string literal") {
            gl();
        }
        else {
            Exp();
        }
    }
}
void Cin() {
    if (c.content_ != ">>") {
        throw std::string("expected to get a symbols >> in line " + std::to_string(c.num_of_string_));
    }
    gl();
    if (c.type_ != "identifier") {
        throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
    }
    gl();
    while (c.content_ == ">>") {
        gl();
        if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
        }
        gl();
    }
}
void Declaration() {
    itv.type_ = "";
    bool fl_gl = false;
    if (!Array(itv)) {
        itv.type_ = c.content_;
        gl();
    }
    if (c.type_ != "identifier") {
        throw std::string("variable decloration was expected in line " + std::to_string(c.num_of_string_));
    }
    itv.identifier_ = c.content_;
    if (current_scope->CheckId(itv.identifier_)) {
        throw std::string("variable is already declared in line " + std::to_string(c.num_of_string_));
    }
    current_scope->PushId(itv);
    gl();
    if (c.content_ == "=") {
        fl_gl = true;
        gl();
        if (c.type_ == "string literal") {
            gl();
        }
        else {
            Exp();
        }
    }
    while (c.content_ == ",") {
        fl_gl = true;
        gl();
        if (c.type_ != "identifier") {
            throw std::string("variable decloration was expected in line " + std::to_string(c.num_of_string_));
        }
        itv.identifier_ = c.content_;
        gl();
        if (c.content_ == "=") {
            gl();
            if (c.type_ == "string literal") {
                gl();
            }
            else {
                Exp();
            }
        }
        if (current_scope->CheckId(itv.identifier_)) {
            throw std::string("variable is already declared " + std::to_string(c.num_of_string_));
        }
        current_scope->PushId(itv);
    }
    if (c.content_ != ";") {
        if (fl_gl == true) {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
        }
        throw std::string("expected to get a expression in line " + std::to_string(c.num_of_string_));
    }
    gl();
}
void Operator() {
    if (c.content_ == "cin") {
        gl();
        Cin();
        if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
        }
        gl();
    }
    else if (c.content_ == "cout"){
        gl();
        Cout();
        if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
        }
        gl();
    }
    else if (c.content_ == "while") {
        gl();
        While();
    }
    else if (c.content_ == "for") {
        gl();
        For();
    }
    else if (c.content_ == "if") {
        gl();
        If();
    }
    else if (c.content_ == "return") {
        gl();
        Return();
        if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
        }
        gl();
    }
    else {
        Exp();
        if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
        }
        gl();
    }
}
void Block() {
    if (c.content_ != "{") {
        throw std::string("expected to get a symbol { in line " + std::to_string(c.num_of_string_));
    }
    gl();
    
    while (c.content_ != "}") {
        if (c.type_ == "end") {
            throw std::string("expected to get a symbol } in line " + std::to_string(c.num_of_string_));
        }
        if (Type_()) {
            Declaration();
        }
        else if (c.type_ == "identifier") {
            gl();
            if (c.content_ == "(" || c.content_ == "++" || c.content_ == "--") {
                if (c.content_ == "(") {
                    gl();
                    Enumeration();
                    if (c.content_ != ")") {
                        "expected to get a symbols )";
                    }
                }
                gl();
                if (c.content_ != ";") {
                    throw std::string("expected to get a symbols ; in line " + std::to_string(c.num_of_string_));
                }
                gl();
            }
            else {
                i -= 2;
                gl();
                Exp9();
                if (c.content_ != ";") {
                    throw std::string("expected to get a symbols ; in line " + std::to_string(c.num_of_string_));
                }
                gl();
            }
        }
        else {
            Operator();
        }
    }
    if (c.content_ != "}") {
        throw std::string("expected to get a symbols } in line " + std::to_string(c.num_of_string_));
    }
    gl();
}
void Function() {
    if (i >= lexemes.size() - 1) {
        throw std::string("the main declaration is missing in line " + std::to_string(c.num_of_string_));
    }
    FunctionType();
    itv.identifier_ = c.content_;
    bool fl_main = 0;
    if (c.content_ == "main") {
        fl_main = 1;
    }
    else {
        if (c.type_ != "identifier") {
            throw std::string("missing function name in line " + std::to_string(c.num_of_string_));
        }
    }
    gl();
    if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
    }
    gl();
    Parameters();
    if (c.content_ != ")") {
        throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
    }
    gl();
    if (current_scope->CheckId(itv.identifier_)) {
        throw std::string("variable is already declared in line" + std::to_string(c.num_of_string_));
    }
    Itv temp_itv = itv;
    current_scope->PushId(itv);
    current_scope->NewScope();
    for (auto i : temp_itv.params_)
    {
        if (current_scope->CheckId(i.identifier_)) {
            throw std::string("variable is already declared in line" + std::to_string(c.num_of_string_));
        }
        current_scope->PushId(i);
    }
    Block();
    current_scope->Back();
    if (!fl_main) {
        Global();
    }  
}
void Global() {
    gl();
    gl();
    if (c.content_ == "(") {
        i -= 3;
        gl();
        Function();
    }
    else {
        i -= 3;
        gl();
        Declaration();
        Global();
    }
}
void Program() {
    gl();
    Global();
    if (c.type_ != "end") {
        throw std::string("extra characters at the end of the program");
    }
}

void SyntaxAnalyzer(std::vector<Lexeme> lexemes_) {
    current_scope = new Vertex_of_tree({ {}, {}, nullptr });
    swap(lexemes, lexemes_);
    try {
        Program();
        if (i < lexemes.size()) {
            throw std::string("characters are expected to enter");
        }
    }
    catch (std::string str) {
        std::cout << str;
    }
}
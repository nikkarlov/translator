#pragma once
#include <iostream>
#include <vector>
#include <stack>
#include "Lexeme.h"
#include "Semantic.h"

Lexeme c;
int i = -1;

Identifier_type_value itv;
std::vector<Lexeme> lexemes;
std::stack<std::string> stack_of_types;
Control_types control_types;

void gl();
void Exp();
void Type(Identifier_type_value& itv1);
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
    if (c.content_ == "[") {
        gl();
        Exp();
        if (c.content_ != "]") {
            throw std::string("expected to get a symbol ] in line " + std::to_string(c.num_of_string_));
        }
        std::string t_name = control_types.PopTypes();
        if (t_name != "ll" && t_name != "int") {
            throw std::string("RRRR not ll not int in line " + std::to_string(c.num_of_string_));
        }
        return true;
    }
    return false;
}
bool Prior2(std::string str = c.content_) {
    if (str == "+" || str == "-" || str == "!") {
        return true;
    }
    return false;
}
bool Prior3(std::string str = c.content_) {
    if (str == "*" || str == "/" || str == "%") {
        return true;
    }
    return false;
}
bool Prior4(std::string str = c.content_) {
    if (str == "+" || str == "-") {
        return true;
    }
    return false;
}
bool Prior5(std::string str = c.content_) {
    if (str == ">" || str == "<" || str == ">=" || str == "<=") {
        return true;
    }
    return false;
}
bool Prior6(std::string str = c.content_) {
    if (str == "==" || str == "!=") {
        return true;
    }
    return false;
}
bool Prior7(std::string str = c.content_) {
    if (str == "&&") {
        return true;
    }
    return false;
}
bool Prior8(std::string str = c.content_) {
    if (str == "||") {
        return true;
    }
    return false;
}
bool Prior9(std::string str = c.content_) {
    if (str == "=" || str == "+=" || str == "-=" || str == "*=" ||
        str == "/=" || str == "%=") {
        return true;
    }
    return false;
}
void gl() {
    i++;
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
bool Array(Identifier_type_value& itv1) {
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
void Type(Identifier_type_value& itv1) {
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
        //Type(itv_params);
        //itv_params.identifier_ = c.content_;

        if (c.type_ != "identifier") {
            throw std::string("");
        }
        gl();
    }
    //itv.params_.push_back(itv_params);
    //itv_params = {};
    while (c.content_ == ",") {
        gl();
        /*Type(itv_params);
        itv_params.identifier_ = c.content_;*/
        if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
        }
        gl();
        //itv.params_.push_back(itv_params);
        //itv_params = {};
    }
}
void Enumeration() {
    if (c.content_ != ")") {
        if (c.type_ == "string literal") {
            control_types.PushTypes("string");
            gl();
        }
        else {
            Exp();
        }
    }
    while (c.content_ == ",") {
        gl();
        if (c.type_ == "string literal") {
            control_types.PushTypes("string");
            gl();
        }
        else {
            Exp();
        }
    }
}
void Exp1() {
    if (c.type_ == "numeric ll literal") {
        control_types.PushTypes("ll");
        gl();
    }
    else if (c.type_ == "numeric ld literal") {
        control_types.PushTypes("ld");
        gl();
    }
    else if (c.type_ == "identifier") {
        control_types.PushTypes(current_scope->GetType(c.content_));
        //std::vector<std::string> vect_param = current_scope->GetParams(c.content_);
        gl();
        if (c.content_ == "(") {
            control_types.PushOperations("(");
            gl();
            Enumeration();
            if (c.content_ != ")") {
                throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
            }
            gl();

            std::string tmp = control_types.PopTypes();
            int i = 0;
            /*while (tmp != "(") {
                if (i >= vect_param.size() || vect_param[i] != tmp) {
                    throw std::string("RRRRRRRR incorrect params " + std::to_string(c.num_of_string_));
                }
                tmp = control_types.PopTypes();
                i++;
            }
            if (i != vect_param.size()) {
                throw std::string("RRRRRRRR incorrect params " + std::to_string(c.num_of_string_));
            }*/
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
        control_types.PushOperations("(");
        gl();
        Exp8();
        if (c.content_ != ")") {
            throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
        }
        std::string tmp = control_types.PopOperations();
        while (tmp != "(") {
            if (!control_types.isUno(tmp)) {
                throw std::string("RRR NDJDJ in line " + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        gl();
    }
}
void Exp2() {
    bool flag = false;
    if (Prior2()) {
        flag = true;
        control_types.PushOperations(c.content_);
        gl();
    }
    Exp1();
    if (flag) {
        std::string tmp = control_types.PopOperations();
        while (Prior2(tmp)) {
            if (!control_types.isUno(tmp)) {
                throw std::string("ERROR IN TYPE UNO" + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        control_types.PushOperations(tmp);
    }
}
void Exp3() {
    Exp2();
    while (Prior3()) {
        control_types.PushOperations(c.content_);
        gl();
        Exp2();
        std::string tmp = control_types.PopOperations();
        while (Prior3(tmp)) {
            if (!control_types.isUno(tmp)) {
                throw std::string("ERROR IN TYPE UNO" + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        control_types.PushOperations(tmp);
    }
}
void Exp4() {
    Exp3();
    while (Prior4()) {
        control_types.PushOperations(c.content_);
        gl();
        Exp3();
        std::string tmp = control_types.PopOperations();
        while (Prior4(tmp)) {
            if (!control_types.isUno(tmp)) {
                throw std::string("ERROR IN TYPE UNO" + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        control_types.PushOperations(tmp);
    }
}
void Exp5() {
    Exp4();
    while (Prior5()) {
        control_types.PushOperations(c.content_);
        gl();
        Exp4();
        std::string tmp = control_types.PopOperations();
        while (Prior5(tmp)) {
            if (!control_types.isUno(tmp)) {
                throw std::string("ERROR IN TYPE UNO" + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        control_types.PushOperations(tmp);
    }
}
void Exp6() {
    Exp5();
    while (Prior6()) {
        control_types.PushOperations(c.content_);
        gl();
        Exp5();
        std::string tmp = control_types.PopOperations();
        while (Prior6(tmp)) {
            if (!control_types.isUno(tmp)) {
                throw std::string("ERROR IN TYPE UNO" + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        control_types.PushOperations(tmp);
    }
}
void Exp7() {
    Exp6();
    while (Prior7()) {
        control_types.PushOperations(c.content_);
        gl();
        Exp6();
        std::string tmp = control_types.PopOperations();
        while (Prior7(tmp)) {
            if (!control_types.isUno(tmp)) {
                throw std::string("ERROR IN TYPE UNO" + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        control_types.PushOperations(tmp);
    }
}
void Exp8() {
    Exp7();
    while (Prior8()) {
        control_types.PushOperations(c.content_);
        gl();
        Exp7();
        std::string tmp = control_types.PopOperations();
        while (Prior8(tmp)) {
            if (!control_types.isUno(tmp)) {
                throw std::string("ERROR IN TYPE UNO" + std::to_string(c.num_of_string_));
            }
            tmp = control_types.PopOperations();
        }
        control_types.PushOperations(tmp);
    }
}
void Exp() {
    Exp8();
}
void Lvalue() {
    if (c.type_ != "identifier") {
        throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
    }
    control_types.PushTypes(current_scope->GetType(c.content_));
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
    control_types.PushOperations(c.content_);
    if (c.content_ == "=") {
        gl();
        if (c.type_ == "string literal") {
            control_types.PushTypes("string");
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
    if (!control_types.isUno(control_types.PopOperations())) {
        throw std::string("RRRRR ww in line " + std::to_string(c.num_of_string_));
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
    Identifier_type_value temp_itv;
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
    /*if (current_scope->CheckId(itv.identifier_)) {
        throw std::string("variable is already declared in line" + std::to_string(c.num_of_string_));
    }*/
    Identifier_type_value temp_itv = itv;
    //current_scope->PushId(itv);
    //current_scope->NewScope();
    /*for (auto i : temp_itv.params_)
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
    } */ 
}
void Global() {
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
    //current_scope = new Vertex({ {}, {}, nullptr });
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
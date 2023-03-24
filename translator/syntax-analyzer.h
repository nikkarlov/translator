#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Lexeme.h"
#include "semantic-analyzer.h"

std::vector<Lexeme> lexemes;
int index = -1;
Scope* current_scope = new Scope();
ControlTypesInExpressions control_types_in_expressions;

void Expression(std::string declared_type = "", std::string ttype = "");

void gl(int i = -1) {
    index++;
    if (index == lexemes.size()) return;
    if (index > lexemes.size()) {
        throw std::string("extra characters in line " + std::to_string(current_lexeme.num_of_string_));
    } else {
        if (i != -1) index = i;
        current_lexeme = lexemes[index];
        if (current_lexeme.type_ == "incorrect symbol") {
            throw std::string("incorrect symbol in line " + std::to_string(current_lexeme.num_of_string_));
        }
    }
}

void Identifier(bool bracket = true, bool mass = 1, bool b = 0) {
    if (current_lexeme.type_ == "identifier") {
        gl();
        if (b || mass && bracket && current_lexeme.content_ == "[") {
            if (current_lexeme.content_ != "[") throw std::string("[ was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            if (current_lexeme.type_ != "int numeric literal" && current_lexeme.type_ != "identifier" ||
                current_lexeme.type_ == "identifier" && current_scope->GetType(current_lexeme.content_) != "int") throw std::string("integer or identifier was expected in index line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
        }
        return;
    }
    throw std::string("identifier was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Expression9(std::string ttype = "");
void Expression11(std::string ttype = "");
void Lvalue(std::string declared_type);

void Expression(std::string declared_type, std::string ttype) {
    int cur_ind = index;
    if (current_lexeme.type_ == "identifier") {
        if (declared_type.substr(0, 4) == "mass") {
            Identifier(1, 1, 1);
        }
        else
        {
            Identifier();
        }
    }
    else
    {
        gl();
    }
    if (current_lexeme.content_ == "+=" || current_lexeme.content_ == "-=" || current_lexeme.content_ == "=" ||
        current_lexeme.content_ == "*=" || current_lexeme.content_ == "/=" || current_lexeme.content_ == "%=" ||
        current_lexeme.content_ == "&=" || current_lexeme.content_ == "|=" || current_lexeme.content_ == "^=" ||
        current_lexeme.content_ == "<<=" || current_lexeme.content_ == ">>=" || current_lexeme.content_ == "~=") {
        gl(cur_ind);
        Lvalue(declared_type);
    }
    else
    {
        gl(cur_ind);
        Expression11(ttype);
    }
    
    if (current_lexeme.content_ == "+=" || current_lexeme.content_ == "-=" || current_lexeme.content_ == "=" ||
        current_lexeme.content_ == "*=" || current_lexeme.content_ == "/=" || current_lexeme.content_ == "%=" ||
        current_lexeme.content_ == "&=" || current_lexeme.content_ == "|=" || current_lexeme.content_ == "^=" ||
        current_lexeme.content_ == "<<=" || current_lexeme.content_ == ">>=" || current_lexeme.content_ == "~=") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression11();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression0(std::string ttype = "") {
    if (current_lexeme.type_ == "identifier") {
        std::string name = current_lexeme.content_;//id and ineeger literal
        if (ttype != "") {
            if (ttype == "mass") {
                Identifier(1, 0);
            }
            else
            {
                Identifier();
            }
            control_types_in_expressions.Push(ttype);
            return;
        }
        if (!current_scope->IsIdentifier(current_lexeme.content_)) throw std::string("variable " + current_lexeme.content_ + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
        Identifier();
        control_types_in_expressions.Push(current_scope->GetType(name));
    } else if (current_lexeme.type_ == "float numeric literal") {
        control_types_in_expressions.Push("float");
        gl();
    } else if (current_lexeme.type_ == "int numeric literal") {
        control_types_in_expressions.Push("int");
        gl();
    } else if (current_lexeme.type_ == "string literal") {
        control_types_in_expressions.Push("string");
        gl();
    } else if (current_lexeme.type_ == "char literal") {
        control_types_in_expressions.Push("char");
        gl();
    } else if (current_lexeme.content_ == "true" || current_lexeme.content_ == "false") {
        control_types_in_expressions.Push("bool");
        gl();
    } else if (current_lexeme.content_ == "(") {
        gl();
        Expression();
        if (current_lexeme.content_ != ")") {
            throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
        }
        gl();
    } else {
        throw std::string("operand expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
}

void Expression1(std::string ttype = "") {
    if (current_lexeme.content_ == "+" || current_lexeme.content_ == "-" || current_lexeme.content_ == "!" || current_lexeme.content_ == "~") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression0();
        control_types_in_expressions.CheckUnary();
    }
    else
    {
        Expression0(ttype);
    }
}

void Expression2(std::string ttype = "") {
    Expression1(ttype);
    while (current_lexeme.content_ == "*" || current_lexeme.content_ == "/" || current_lexeme.content_ == "%") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression1();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression3(std::string ttype = "") {
    Expression2(ttype);
    while (current_lexeme.content_ == "+" || current_lexeme.content_ == "-") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression2();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression4(std::string ttype = "") {
    Expression3(ttype);
    while (current_lexeme.content_ == ">>" || current_lexeme.content_ == "<<") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression3();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression5(std::string ttype = "") {
    Expression4(ttype);
    if (current_lexeme.content_ == ">" || current_lexeme.content_ == "<" || current_lexeme.content_ == ">=" || current_lexeme.content_ == "<=") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression4();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression6(std::string ttype = "") {
    Expression5(ttype);
    if (current_lexeme.content_ == "==" || current_lexeme.content_ == "!=") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression5();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression7(std::string ttype = "") {
    Expression6(ttype);
    while (current_lexeme.content_ == "&") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression6();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression8(std::string ttype = "") {
    Expression7(ttype);
    while (current_lexeme.content_ == "^") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression7();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression9(std::string ttype) {
    Expression8(ttype);
    while (current_lexeme.content_ == "|") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression8();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression10(std::string ttype = "") {
    Expression9(ttype);
    while (current_lexeme.content_ == "&&") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression9();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression11(std::string ttype) {
    Expression10(ttype);
    while (current_lexeme.content_ == "||") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression10();
        control_types_in_expressions.CheckBinary();
    }
}

void Lvalue(std::string declared_type) {
    if (current_lexeme.type_ != "identifier") throw std::string("the left operand of the operation = must be an identifier in line " + std::to_string(current_lexeme.num_of_string_));
    if (declared_type == "") {
        control_types_in_expressions.Push(current_scope->GetType(current_lexeme.content_));
    }
    else
    {
        control_types_in_expressions.Push(declared_type);
    }
    Identifier();
}

bool CorrectType(std::string str, bool mass = true, bool string = true) {
    return str == "bool" || str == "char" || string && str == "string" || str == "int" ||
        str == "longlong" || str == "float" || str == "double" || mass && str == "mass";
}

std::vector<std::string> Parametres() {
    std::vector<std::string> types;
    current_scope = current_scope->CreateNewScope();
    if (current_lexeme.content_ == ")") return types;
    if (!CorrectType(current_lexeme.content_, 0)) throw std::string("invalid type in the parameter in line " + std::to_string(current_lexeme.num_of_string_));
    std::string type = current_lexeme.content_;
    gl();
    std::string name = current_lexeme.content_;
    Identifier();
    if (!current_scope->IsIdentifier(name)) {
        types.push_back(type);
        current_scope->PushIndetifier(type, name);
    } else {
        throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
    }
    while (current_lexeme.content_ != ")") {
        if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (!CorrectType(current_lexeme.content_, 0)) throw std::string("invalid type in the parameter in line " + std::to_string(current_lexeme.num_of_string_));
        std::string type = current_lexeme.content_;
        gl();
        std::string name = current_lexeme.content_;
        Identifier();
        if (!current_scope->IsIdentifier(name)) {
            types.push_back(type);
            current_scope->PushIndetifier(type, name);
        }
        else {
            throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
        }
    }
    return types;
}

void Cin() {
    gl();
    if (current_lexeme.content_ != ">>") throw std::string(">> was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (!current_scope->IsIdentifier(current_lexeme.content_)) throw std::string("variable " + current_lexeme.content_ + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
    Identifier();
    while (current_lexeme.content_ == ">>") {
        gl();
        if (!current_scope->IsIdentifier(current_lexeme.content_)) throw std::string("variable " + current_lexeme.content_ + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
        Identifier();
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void Cout() {
    gl();
    if (current_lexeme.content_ != "<<") throw std::string("<< was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Expression();
    control_types_in_expressions.GetTypeFromStack();
    while (current_lexeme.content_ == "<<") {
        gl();
        Expression();
        control_types_in_expressions.GetTypeFromStack();
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void Local(std::string function_name);

void Block(bool new_scope = true, std::string function_name = "") {
    if (current_lexeme.content_ != "{") {
        throw std::string("{ was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    if (new_scope) current_scope = current_scope->CreateNewScope();
    while (current_lexeme.content_ != "}") {
        Local(function_name);
    }
    if (current_lexeme.content_ != "}") {
        throw std::string("} was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    current_scope = current_scope->BackPreviousScope();
}

void If() {
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Expression();
    if (control_types_in_expressions.GetTypeFromStack() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
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
    Expression();
    if (control_types_in_expressions.GetTypeFromStack() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Block();
}

void For() {
    gl();
    current_scope = current_scope->CreateNewScope();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ";") {
        std::string type, name;
        bool bdec = false;
        if (current_lexeme.type_ == "official word") {
            if (!CorrectType(current_lexeme.content_, 0, 0)) throw std::string("invalid type in line " + std::to_string(current_lexeme.num_of_string_));
            type = current_lexeme.content_;
            gl();
            name = current_lexeme.content_;
            Expression(type, type);
            bdec = true;
        } else {
            name = current_lexeme.content_;
            type = current_scope->GetType(name);
            Expression(type, type);
        }
        control_types_in_expressions.GetTypeFromStack();
        if (bdec) {
            if (!current_scope->IsIdentifier(name)) {
                current_scope->PushIndetifier(type, name);
            } else {
                throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
            }
        } else {
            if (!current_scope->IsIdentifier(name)) throw std::string("variable " + name + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
        }
        while (current_lexeme.content_ != ";") {
            if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            std::string type, name;
            bool bdec = false;
            if (current_lexeme.type_ == "official word") {
                if (!CorrectType(current_lexeme.content_, 0, 0)) throw std::string("invalid type in line " + std::to_string(current_lexeme.num_of_string_));
                type = current_lexeme.content_;
                gl();
                name = current_lexeme.content_;
                Expression(type, type);
                bdec = true;
            }
            else {
                name = current_lexeme.content_;
                type = current_scope->GetType(name);
                Expression(type, type);
            }
            control_types_in_expressions.GetTypeFromStack();
            if (bdec) {
                if (!current_scope->IsIdentifier(name)) {
                    current_scope->PushIndetifier(type, name);
                }
                else {
                    throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
                }
            }
            else {
                if (!current_scope->IsIdentifier(name)) throw std::string("variable " + name + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
            }
        }
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ";") {
        Expression();
        if (control_types_in_expressions.GetTypeFromStack() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ")") {
        if (current_lexeme.type_ != "identifier") throw std::string("lvalue must be identifier" + std::to_string(current_lexeme.num_of_string_));
        if (!current_scope->IsIdentifier(current_lexeme.content_)) throw std::string("variable is not declared in line " + std::to_string(current_lexeme.num_of_string_));
        Expression(current_scope->GetType(current_lexeme.content_));
        control_types_in_expressions.GetTypeFromStack();
        while (current_lexeme.content_ != ")") {
            if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));;
            gl();
            if (current_lexeme.type_ != "identifier") throw std::string("lvalue must be identifier" + std::to_string(current_lexeme.num_of_string_));
            if (!current_scope->IsIdentifier(current_lexeme.content_)) throw std::string("variable is not declared in line " + std::to_string(current_lexeme.num_of_string_));
            Expression(current_scope->GetType(current_lexeme.content_));
            control_types_in_expressions.GetTypeFromStack();
        }
    }
    gl();
    Block(false);
}

void VariableDecloration() {
    std::string type = current_lexeme.content_, masstype;
    if (current_lexeme.content_ == "mass") {  //typeeeeeeee
        gl();
        if (current_lexeme.content_ != "<") throw std::string("type od mass ne ykazan was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (!CorrectType(current_lexeme.content_, 0)) throw std::string("incorrect type od mass in line " + std::to_string(current_lexeme.num_of_string_));
        masstype = current_lexeme.content_;
        gl();
        if (current_lexeme.content_ != ">") throw std::string("type od mass ne ykazan was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    std::string name = current_lexeme.content_;
    Expression(type, type);
    control_types_in_expressions.GetTypeFromStack();
    if (!current_scope->IsIdentifierMe(name)) {
        if (type == "mass") {
            if (current_lexeme.content_ != "[") throw std::string("[ was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            Expression();
            if (control_types_in_expressions.GetTypeFromStack() != "int") throw std::string("must be int in [] in line " + std::to_string(current_lexeme.num_of_string_));
            if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            current_scope->PushIndetifier(type + masstype, name);
        }
        else
        {
            current_scope->PushIndetifier(type, name);
        }
    } else {
        throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
    }
    while (current_lexeme.content_ == ",") {
        gl();
        name = current_lexeme.content_;
        Expression(type, type);
        control_types_in_expressions.GetTypeFromStack();
        if (!current_scope->IsIdentifier(name)) {
            current_scope->PushIndetifier(type, name);
        }
        else {
            throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
        }
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void Return(std::string function_name) {
    gl();
    if (current_lexeme.content_ == ";") {
        gl();
        return;
    }
    Expression(current_scope->GetType(current_lexeme.content_));
    if (control_types_in_expressions.GetTypeFromStack() !=  table_of_functions[function_name].type_) throw std::string("the type of the returned value does not match the type of the function in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void FunctionCall(bool vlozh = false) {
    std::string name = current_lexeme.content_;
    std::vector<std::string> types;
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ")") {
        if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "(") {
            FunctionCall(true);
            types.push_back(table_of_functions[name].type_);
        }
        else if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "[") {
            std::string name = current_lexeme.content_;
            gl();
            gl();
            Expression();
            gl();
            types.push_back(current_scope->GetType(name).substr(4));
        }
        else
        {
            Expression();
            types.push_back(control_types_in_expressions.GetTypeFromStack());
        }
        while (current_lexeme.content_ == ",") {
            gl();
            if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "(") {
                FunctionCall(true);
                types.push_back(table_of_functions[name].type_);
            }
            else if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "[") {
                std::string name = current_lexeme.content_;
                gl();
                gl();
                Expression();
                gl();
                types.push_back(current_scope->GetType(name).substr(4));
            }
            else
            {
                Expression();
                types.push_back(control_types_in_expressions.GetTypeFromStack());
            }
        }
    }
    std::string error = IsFunction(name, types);
    if (error != "ok") throw std::string(error + "in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();

    if (!vlozh) {
        if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
    }
}

void Local(std::string function_name = "") {
    if (CorrectType(current_lexeme.content_)) {
        VariableDecloration();
        return;
    }
    if (current_lexeme.type_ == "identifier") {
        if (lexemes[index + 1].content_ == "(") {
            FunctionCall();
            return;
        }
        Expression(current_scope->GetType(current_lexeme.content_));
        control_types_in_expressions.GetTypeFromStack();
        if (current_lexeme.content_ != ";") {
            throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
        }
        gl();
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
    if (current_lexeme.content_ == "return") {
        Return(function_name);
        return;
    }
    throw std::string("Variable decloration or assignment or .... was expected in line " + std::to_string(current_lexeme.num_of_string_));
}//JI3EWJFFFFFFFFFFFFFFFFFFF

void FunctionDecloration() {
    std::string type = current_lexeme.content_;
    gl();
    std::string name = current_lexeme.content_;
    Identifier();
    if (current_lexeme.content_ != "(") {
        throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    if (!addFunction(type, name, Parametres())) {
        throw std::string("redefining the function " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
    }
    if (current_lexeme.content_ != ")") {
        throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    Block(false, name);
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
        if (current_lexeme.content_ == "=" || current_lexeme.content_ == ";" || current_lexeme.content_ == ",") {
            gl(index_of_type);
            VariableDecloration();
            return;
        }
    }
    throw std::string("variable or function decloration was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Global() {
    if (current_lexeme.type_ == "official word") {
        Decloration();
        return;
    }
    if (current_lexeme.type_ == "identifier") {
        Expression();
        control_types_in_expressions.GetTypeFromStack();
        if (current_lexeme.content_ != ";") {
            throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
        }
        gl();
        return;
    }
    throw std::string("decloration or assignment was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Program() {
    gl();
    while (index != lexemes.size()) {
        Global();
    }
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

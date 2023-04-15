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

std::string Expression12();

void Identifier() {
    if (current_lexeme.type_ == "identifier") {
        if (current_scope->IsIdentifier(current_lexeme.content_) && current_scope->GetType(current_lexeme.content_).substr(0, 4) == "list") {
            gl();
            if (current_lexeme.content_ != "[") throw std::string("[ was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            if (Expression12() != "int") throw std::string("the expression must be of the integer type in line " + std::to_string(current_lexeme.num_of_string_));
            if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            return;
        }
        gl();
        return;
    }
    throw std::string("identifier was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Expression0() {
    if (current_lexeme.type_ == "identifier") {
        std::string name = current_lexeme.content_;
        if (!current_scope->IsIdentifier(name)) throw std::string("variable " + name + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
        control_types_in_expressions.Push(current_scope->GetType(name).substr(0, 4) == "list" ? current_scope->GetType(name).substr(4) : current_scope->GetType(name));
        Identifier();
        return;
    }
    if (current_lexeme.type_ == "float numeric literal") {
        control_types_in_expressions.Push("float");
        gl();
        return;
    }
    if (current_lexeme.type_ == "int numeric literal") {
        control_types_in_expressions.Push("int");
        gl();
        return;
    }
    if (current_lexeme.type_ == "string literal") {
        control_types_in_expressions.Push("string");
        gl();
        return;
    }
    if (current_lexeme.type_ == "char literal") {
        control_types_in_expressions.Push("char");
        gl();
        return;
    }
    if (current_lexeme.content_ == "true" || current_lexeme.content_ == "false") {
        control_types_in_expressions.Push("bool");
        gl();
        return;
    }
    if (current_lexeme.content_ == "(") {
        gl();
        control_types_in_expressions.Push(Expression12());
        if (current_lexeme.content_ != ")") {
            throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
        }
        gl();
        return;
    }
    throw std::string("operand expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Expression1() {
    if (current_lexeme.content_ == "+" || current_lexeme.content_ == "-" || current_lexeme.content_ == "!" || current_lexeme.content_ == "~") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression0();
        control_types_in_expressions.CheckUnary();
    }
    else
    {
        Expression0();
    }
}

void Expression2() {
    Expression1();
    while (current_lexeme.content_ == "*" || current_lexeme.content_ == "/" || current_lexeme.content_ == "%") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression1();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression3() {
    Expression2();
    while (current_lexeme.content_ == "+" || current_lexeme.content_ == "-") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression2();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression4() {
    Expression3();
    while (current_lexeme.content_ == ">>" || current_lexeme.content_ == "<<") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression3();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression5() {
    Expression4();
    if (current_lexeme.content_ == ">" || current_lexeme.content_ == "<" || current_lexeme.content_ == ">=" || current_lexeme.content_ == "<=") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression4();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression6() {
    Expression5();
    if (current_lexeme.content_ == "==" || current_lexeme.content_ == "!=") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression5();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression7() {
    Expression6();
    while (current_lexeme.content_ == "&") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression6();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression8() {
    Expression7();
    while (current_lexeme.content_ == "^") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression7();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression9() {
    Expression8();
    while (current_lexeme.content_ == "|") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression8();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression10() {
    Expression9();
    while (current_lexeme.content_ == "&&") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression9();
        control_types_in_expressions.CheckBinary();
    }
}

void Expression11() {
    Expression10();
    while (current_lexeme.content_ == "||") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression10();
        control_types_in_expressions.CheckBinary();
    }
}

bool CheckAssignment() {
    int current_index = index;
    try
    {
        Identifier();
        if (lexemes[index].content_ == "+=" || lexemes[index].content_ == "-=" || lexemes[index].content_ == "=" ||
            lexemes[index].content_ == "*=" || lexemes[index].content_ == "/=" || lexemes[index].content_ == "%=" ||
            lexemes[index].content_ == "&=" || lexemes[index].content_ == "|=" || lexemes[index].content_ == "^=" ||
            lexemes[index].content_ == "<<=" || lexemes[index].content_ == ">>=" || lexemes[index].content_ == "~=") {
            gl(current_index);
            return true;
        }
        gl(current_index);
        return false;
    }
    catch (std::string str)
    {
        gl(current_index);
        return false;
    }
}

std::string Expression12() {
    if (!CheckAssignment()) {
        Expression11();
        return control_types_in_expressions.GetTypeFromStack();
    }
    std::string name_identifier = current_lexeme.content_;
    Identifier();
    if (!current_scope->IsIdentifier(name_identifier)) throw std::string("variable " + name_identifier + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
    control_types_in_expressions.Push(current_scope->GetType(name_identifier));
    if (lexemes[index].content_ == "+=" || lexemes[index].content_ == "-=" || lexemes[index].content_ == "=" ||
        lexemes[index].content_ == "*=" || lexemes[index].content_ == "/=" || lexemes[index].content_ == "%=" ||
        lexemes[index].content_ == "&=" || lexemes[index].content_ == "|=" || lexemes[index].content_ == "^=" ||
        lexemes[index].content_ == "<<=" || lexemes[index].content_ == ">>=" || lexemes[index].content_ == "~=") {
        control_types_in_expressions.Push(current_lexeme.content_);
        gl();
        Expression11();
        control_types_in_expressions.CheckBinary();
    }
    return control_types_in_expressions.GetTypeFromStack();
}

bool CorrectType(std::string str, bool list = true, bool string = true) {
    return str == "bool" || str == "char" || string && str == "string" || str == "int" ||
        str == "longlong" || str == "float" || str == "double" || list && str == "list";
}

std::vector<std::string> Parametres(std::string function_name) {
    std::vector<std::string> types_of_params;
    current_scope = current_scope->CreateNewScope(function_name);
    if (current_lexeme.content_ == ")") return types_of_params;
    if (!CorrectType(current_lexeme.content_)) throw std::string("invalid type in the parameter in line " + std::to_string(current_lexeme.num_of_string_));
    std::string type = current_lexeme.content_;
    gl();
    std::string name = current_lexeme.content_;
    Identifier();
    if (!current_scope->IsIdentifier(name)) {
        types_of_params.push_back(type);
        current_scope->PushIndetifier(type, name);
    } else {
        throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
    }
    while (current_lexeme.content_ != ")") {
        if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (!CorrectType(current_lexeme.content_)) throw std::string("invalid type in the parameter in line " + std::to_string(current_lexeme.num_of_string_));
        std::string type = current_lexeme.content_;
        gl();
        std::string name = current_lexeme.content_;
        Identifier();
        if (!current_scope->IsIdentifier(name)) {
            types_of_params.push_back(type);
            current_scope->PushIndetifier(type, name);
        }
        else {
            throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
        }
    }
    return types_of_params;
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
    Expression12();
    while (current_lexeme.content_ == "<<") {
        gl();
        Expression12();
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void Local();

void Block(bool new_scope = true) {
    if (current_lexeme.content_ != "{") {
        throw std::string("{ was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    if (new_scope) current_scope = current_scope->CreateNewScope(current_scope->GetFunctionName());
    while (current_lexeme.content_ != "}") {
        Local();
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
    if (Expression12() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
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
    if (Expression12() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Block();
}

void For() {
    gl();
    current_scope = current_scope->CreateNewScope(current_scope->GetFunctionName());
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ";") {
        std::string type_initialized_variables, name_initialized_variables;
        if (current_lexeme.type_ == "official word") {
            if (!CorrectType(current_lexeme.content_, 0, 0)) throw std::string("invalid type in line " + std::to_string(current_lexeme.num_of_string_));
            type_initialized_variables = current_lexeme.content_;
            gl();
            name_initialized_variables = current_lexeme.content_;
            if (!current_scope->IsIdentifierMe(name_initialized_variables)) {
                current_scope->PushIndetifier(type_initialized_variables, name_initialized_variables);
            }
        }
        Expression12();
        while (current_lexeme.content_ != ";") {
            if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            if (current_lexeme.type_ == "official word") {
                if (!CorrectType(current_lexeme.content_, 0, 0)) throw std::string("invalid type in line " + std::to_string(current_lexeme.num_of_string_));
                type_initialized_variables = current_lexeme.content_;
                gl();
                name_initialized_variables = current_lexeme.content_;
                if (!current_scope->IsIdentifierMe(name_initialized_variables)) {
                    current_scope->PushIndetifier(type_initialized_variables, name_initialized_variables);
                }
            }
            Expression12();
        }
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    //  ---------------------------------------------------------------------------------------------
    if (current_lexeme.content_ != ";") {   //  булевское выражение
        if (Expression12() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ")") {
        Expression12();
        while (current_lexeme.content_ != ")") {
            if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            Expression12();
        }
    }
    gl();
    Block(false);
}

void VariableDecloration() {
    std::string type = current_lexeme.content_, type_of_list;
    bool isList = false;
    if (current_lexeme.content_ == "list") {
        isList = true;
        gl();
        if (current_lexeme.content_ != "<") throw std::string("< was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (!CorrectType(current_lexeme.content_, 0)) throw std::string("the expression must be of the integer type in line " + std::to_string(current_lexeme.num_of_string_));
        type_of_list = current_lexeme.content_;
        gl();
        if (current_lexeme.content_ != ">") throw std::string("> was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    std::string name_of_variable = current_lexeme.content_;
    if (current_scope->IsIdentifierMe(name_of_variable)) throw std::string("redefining the variable " + name_of_variable + " in line " + std::to_string(current_lexeme.num_of_string_));
    if (isList) {
        current_scope->PushIndetifier(type + type_of_list, name_of_variable);
        gl();
        if (current_lexeme.content_ != "[") throw std::string("[ was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (Expression12() != "int") throw std::string("the expression must be of the integer type in line " + std::to_string(current_lexeme.num_of_string_));
        if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
    }
    else
    {
        current_scope->PushIndetifier(type, name_of_variable);
        Expression12();
    }
    while (current_lexeme.content_ == ",") {
        gl();
        name_of_variable = current_lexeme.content_;
        if (current_scope->IsIdentifierMe(name_of_variable)) throw std::string("redefining the variable " + name_of_variable + " in line " + std::to_string(current_lexeme.num_of_string_));
        if (isList) {
            current_scope->PushIndetifier(type + type_of_list, name_of_variable);
        }
        else
        {
            current_scope->PushIndetifier(type, name_of_variable);
        }
        Expression12();
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
    if (Expression12() !=  table_of_functions[function_name].type_) throw std::string("the type of the returned value does not match the type of the function in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void FunctionCall(bool nested_function = false) {
    std::string function_name = current_lexeme.content_;
    std::vector<std::string> types_function_arguments;
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ")") {
        if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "(") {
            std::string name_of_function_in_args = current_lexeme.content_;
            FunctionCall(true);
            types_function_arguments.push_back(table_of_functions[name_of_function_in_args].type_);
        }
        else if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "[") {
            std::string name_of_list_in_args = current_lexeme.content_;
            gl();
            gl();
            if (Expression12() != "int") throw std::string("the expression must be of the integer type in line " + std::to_string(current_lexeme.num_of_string_));
            if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            types_function_arguments.push_back(current_scope->GetType(name_of_list_in_args).substr(4));
        }
        else
        {
            types_function_arguments.push_back(Expression12());
        }
        while (current_lexeme.content_ == ",") {
            gl();
            if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "(") {
                std::string name_of_function_in_args = current_lexeme.content_;
                FunctionCall(true);
                types_function_arguments.push_back(table_of_functions[name_of_function_in_args].type_);
            }
            else if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "[") {
                std::string name_of_list_in_args = current_lexeme.content_;
                gl();
                gl();
                if (Expression12() != "int") throw std::string("the expression must be of the integer type in line " + std::to_string(current_lexeme.num_of_string_));
                if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
                gl();
                types_function_arguments.push_back(current_scope->GetType(name_of_list_in_args).substr(4));
            }
            else
            {
                types_function_arguments.push_back(Expression12());
            }
        }
    }
    std::string error = CheckCorrectFunction(function_name, types_function_arguments);
    if (error != "ok") throw std::string(error + "in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (!nested_function) {
        if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
    }
}

void Local() {
    if (CorrectType(current_lexeme.content_)) {
        VariableDecloration();
        return;
    }
    if (current_lexeme.type_ == "identifier") {
        if (lexemes[index + 1].content_ == "(") {
            FunctionCall();
            return;
        }
        Expression12();
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
        Return(current_scope->GetFunctionName());
        return;
    }
    throw std::string("operation was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void FunctionDecloration() {
    std::string function_type = current_lexeme.content_;
    gl();
    std::string function_name = current_lexeme.content_;
    Identifier();
    if (current_lexeme.content_ != "(") {
        throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    if (!AddFunction(function_type, function_name, Parametres(function_name))) {
        throw std::string("redefining the function " + function_name + " in line " + std::to_string(current_lexeme.num_of_string_));
    }
    if (current_lexeme.content_ != ")") {
        throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    }
    gl();
    Block(false);
}

void Global() {
    if (current_lexeme.type_ != "official word") throw std::string("variable or function decloration was expected in line " + std::to_string(current_lexeme.num_of_string_));
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
        std::cout << "ok";
    }
    catch (std::string str) {
        std::cout << str;
    }
}

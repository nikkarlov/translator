#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "Lexeme.h"
#include "semantic-analyzer.h"
#include "poliz-generator.h"

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

std::string Expression12(bool assignment = true);

void Identifier(bool poliz_push = true) {
    std::string identifier_name = current_lexeme.content_;
    if (current_lexeme.type_ == "identifier") {
        if (current_scope->IsIdentifier(current_lexeme.content_) && current_scope->GetType(current_lexeme.content_).substr(0, 4) == "list") {
            gl();
            if (current_lexeme.content_ != "[") throw std::string("[ was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            if (Expression12() != "int") throw std::string("the expression must be of the integer type in line " + std::to_string(current_lexeme.num_of_string_));
            if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            if (poliz_push) poliz.Push(identifier_name);
            if (poliz_push) poliz.Push("[]");
            return;
        }
        if (poliz_push) poliz.Push("variable_" + identifier_name);
        gl();
        return;
    }
    throw std::string("identifier was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void FunctionCall();

void Expression0() {
    if (isFunction(current_lexeme.content_)) {
        std::string name = current_lexeme.content_;
        FunctionCall();
        control_types_in_expressions.Push(table_of_functions[name].type_);
        return;
    }
    if (current_lexeme.type_ == "identifier") {
        std::string name = current_lexeme.content_;
        if (!current_scope->IsIdentifier(name)) throw std::string("variable " + name + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
        control_types_in_expressions.Push(current_scope->GetType(name) == "list" ? current_scope->GetType(name, true) : current_scope->GetType(name));
        Identifier();
        return;
    }
    if (current_lexeme.type_ == "float numeric literal") {
        control_types_in_expressions.Push("float");
        poliz.Push(current_lexeme.content_);
        gl();
        return;
    }
    if (current_lexeme.type_ == "int numeric literal") {
        control_types_in_expressions.Push("int");
        poliz.Push(current_lexeme.content_);
        gl();
        return;
    }
    if (current_lexeme.type_ == "string literal") {
        control_types_in_expressions.Push("string");
        poliz.Push(current_lexeme.content_);
        gl();
        return;
    }
    if (current_lexeme.type_ == "char literal") {
        control_types_in_expressions.Push("char");
        poliz.Push(current_lexeme.content_);
        gl();
        return;
    }
    if (current_lexeme.content_ == "true" || current_lexeme.content_ == "false") {
        control_types_in_expressions.Push("bool");
        poliz.Push(current_lexeme.content_);
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
    poliz.Disable();
    int current_index = index;
    try
    {
        Identifier(false);
        if (lexemes[index].content_ == "+=" || lexemes[index].content_ == "-=" || lexemes[index].content_ == "=" ||
            lexemes[index].content_ == "*=" || lexemes[index].content_ == "/=" || lexemes[index].content_ == "%=" ||
            lexemes[index].content_ == "&=" || lexemes[index].content_ == "|=" || lexemes[index].content_ == "^=" ||
            lexemes[index].content_ == "<<=" || lexemes[index].content_ == ">>=" || lexemes[index].content_ == "~=") {
            gl(current_index);
            poliz.Enable();
            return true;
        }
        gl(current_index);
        poliz.Enable();
        return false;
    }
    catch (std::string str)
    {
        gl(current_index);
        poliz.Enable();
        return false;
    }
}

std::string Expression12(bool assignment) {
    if (!CheckAssignment()) {
        if (!assignment) poliz.Disable();
        Expression11();
        if (!assignment) poliz.Enable();
        return control_types_in_expressions.GetTypeFromStack();
    }
    std::string name_identifier = current_lexeme.content_;
    Identifier();
    poliz.Push("adress");
    if (!current_scope->IsIdentifier(name_identifier)) throw std::string("variable " + name_identifier + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
    control_types_in_expressions.Push(current_scope->GetType(name_identifier) == "list" ? current_scope->GetType(name_identifier, true) : current_scope->GetType(name_identifier));
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
    poliz.Push("adress");
    poliz.Push("ADRESS POLIZ");
    poliz.Push("=");
    if (!current_scope->IsIdentifierMe(name)) {
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
        poliz.Push("adress");
        poliz.Push("ADRESS POLIZ");
        poliz.Push("=");
        if (!current_scope->IsIdentifierMe(name)) {
            types_of_params.push_back(type);
            current_scope->PushIndetifier(type, name);
        }
        else {
            throw std::string("redefining the variable " + name + " in line " + std::to_string(current_lexeme.num_of_string_));
        }
    }
    return types_of_params;
}

void Input() {
    gl();
    if (current_lexeme.content_ != ">>") throw std::string(">> was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (!current_scope->IsIdentifier(current_lexeme.content_)) throw std::string("variable " + current_lexeme.content_ + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
    Identifier();
    poliz.Push("adress");
    poliz.Push("INPUT");
    poliz.Push("=");
    while (current_lexeme.content_ == ">>") {
        gl();
        if (!current_scope->IsIdentifier(current_lexeme.content_)) throw std::string("variable " + current_lexeme.content_ + " is not declared in line " + std::to_string(current_lexeme.num_of_string_));
        Identifier();
        poliz.Push("adress");
        poliz.Push("INPUT");
        poliz.Push("=");
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void Print() {
    int count = 1;
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    Expression12();
    while (current_lexeme.content_ != ")") {
        if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        Expression12();
        count++;
    }
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    poliz.Push(std::to_string(count));
    poliz.Push("print");
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
    int missing_condition_address = poliz.GetCurrentIndex();
    poliz.SetBlankSpace();
    poliz.Push("!F");
    Block();
    int missing_address_end_condition = poliz.GetCurrentIndex();
    poliz.SetBlankSpace();
    poliz.Push("!");
    poliz.SetAdressInBlankSpace(missing_condition_address, poliz.GetCurrentIndex());
    if (current_lexeme.content_ == "else") {
        gl();
        Block();
    }
    poliz.SetAdressInBlankSpace(missing_address_end_condition, poliz.GetCurrentIndex());
}

void While() {
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    int start_index_condition = poliz.GetCurrentIndex();
    if (Expression12() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ")") throw std::string(") was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    int missing_adress_end_while = poliz.GetCurrentIndex();
    poliz.SetBlankSpace();
    poliz.Push("!F");
    Block();
    poliz.SetBlankSpace();
    poliz.SetAdressInBlankSpace(poliz.GetCurrentIndex() - 1, start_index_condition);
    poliz.Push("!");
    poliz.SetAdressInBlankSpace(missing_adress_end_while, poliz.GetCurrentIndex());
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
    int start_index_condition = poliz.GetCurrentIndex();
    if (current_lexeme.content_ != ";") {   //  булевское выражение
        if (Expression12() != "bool") throw std::string("the expression must be of the boolean type in line " + std::to_string(current_lexeme.num_of_string_));
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    int missing_adress_end_for = poliz.GetCurrentIndex();
    poliz.SetBlankSpace();
    poliz.Push("!F");
    int missng_adress_block = poliz.GetCurrentIndex();
    poliz.SetBlankSpace();
    poliz.Push("!");
    int start_index_assignment = poliz.GetCurrentIndex();
    if (current_lexeme.content_ != ")") {
        Expression12();
        while (current_lexeme.content_ != ")") {
            if (current_lexeme.content_ != ",") throw std::string(", was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            Expression12();
        }
    }
    poliz.SetBlankSpace();
    poliz.SetAdressInBlankSpace(poliz.GetCurrentIndex() - 1, start_index_condition);
    poliz.Push("!");
    gl();
    poliz.SetAdressInBlankSpace(missng_adress_block, poliz.GetCurrentIndex());
    Block(false);
    poliz.SetBlankSpace();
    poliz.SetAdressInBlankSpace(poliz.GetCurrentIndex() - 1, start_index_assignment);
    poliz.Push("!");
    poliz.SetAdressInBlankSpace(missing_adress_end_for, poliz.GetCurrentIndex());
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
        current_scope->PushIndetifier(type, name_of_variable, type_of_list);
        gl();
        if (current_lexeme.content_ != "[") throw std::string("[ was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        if (Expression12() != "int") throw std::string("the expression must be of the integer type in line " + std::to_string(current_lexeme.num_of_string_));
        if (current_lexeme.content_ != "]") throw std::string("] was expected in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        poliz.Push(name_of_variable);
        poliz.Push("list_size");
    }
    else
    {
        current_scope->PushIndetifier(type, name_of_variable);
        Expression12(false);
    }
    while (current_lexeme.content_ == ",") {
        gl();
        name_of_variable = current_lexeme.content_;
        if (current_scope->IsIdentifierMe(name_of_variable)) throw std::string("redefining the variable " + name_of_variable + " in line " + std::to_string(current_lexeme.num_of_string_));
        if (isList) {
            current_scope->PushIndetifier(type, name_of_variable, type_of_list);
        }
        else
        {
            current_scope->PushIndetifier(type, name_of_variable);
        }
        Expression12(false);
    }
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
}

void Return(std::string function_name) {
    gl();
    if (current_lexeme.content_ == ";") {
        if (table_of_functions[function_name].type_ != "void") throw std::string("the type of the returned value does not match the type of the function in line " + std::to_string(current_lexeme.num_of_string_));
        gl();
        return;
    }
    if (Expression12() !=  table_of_functions[function_name].type_) throw std::string("the type of the returned value does not match the type of the function in line " + std::to_string(current_lexeme.num_of_string_));
    if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    poliz.Push("ADRESS VOZVRATA");
    poliz.Push("!");
}

void FunctionCall() {
    std::string function_name = current_lexeme.content_;
    std::vector<std::string> types_function_arguments;
    gl();
    if (current_lexeme.content_ != "(") throw std::string("( was expected in line " + std::to_string(current_lexeme.num_of_string_));
    gl();
    if (current_lexeme.content_ != ")") {
        if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "(") {
            std::string name_of_function_in_args = current_lexeme.content_;
            FunctionCall();
            types_function_arguments.push_back(table_of_functions[name_of_function_in_args].type_);
        }
        else if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "[") {
            std::string name_of_list_in_args = current_lexeme.content_;
            Identifier();
            types_function_arguments.push_back(current_scope->GetType(name_of_list_in_args) == "list" ? current_scope->GetType(name_of_list_in_args, true) : current_scope->GetType(name_of_list_in_args));
        }
        else
        {
            types_function_arguments.push_back(Expression12());
        }
        while (current_lexeme.content_ == ",") {
            gl();
            if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "(") {
                std::string name_of_function_in_args = current_lexeme.content_;
                FunctionCall();
                types_function_arguments.push_back(table_of_functions[name_of_function_in_args].type_);
            }
            else if (current_lexeme.type_ == "identifier" && lexemes[index + 1].content_ == "[") {
                std::string name_of_list_in_args = current_lexeme.content_;
                Identifier();
                types_function_arguments.push_back(current_scope->GetType(name_of_list_in_args) == "list" ? current_scope->GetType(name_of_list_in_args, true) : current_scope->GetType(name_of_list_in_args));
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
    poliz.Push("function_call_" + function_name);
}
void Local() {
    if (CorrectType(current_lexeme.content_)) {
        VariableDecloration();
        return;
    }
    if (current_lexeme.type_ == "identifier") {
        if (lexemes[index + 1].content_ == "(") {
            FunctionCall();
            if (current_lexeme.content_ != ";") throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
            gl();
            return;
        }
        Expression12();
        if (current_lexeme.content_ != ";") {
            throw std::string("; was expected in line " + std::to_string(current_lexeme.num_of_string_));
        }
        gl();
        return;
    }
    if (current_lexeme.content_ == "input") {
        Input();
        return;
    }
    if (current_lexeme.content_ == "print") {
        Print();
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
    poliz.SetPossibleSize();
    int missing_adress_start = poliz.GetCurrentIndex();
    poliz.SetBlankSpace();
    poliz.Push("!");
    int adress_of_start_function = poliz.GetCurrentIndex();
    //std::cout << ("start of the function" + std::to_string(poliz.GetIndex())) << '\n';
    Identifier(false);
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
    if (current_lexeme.content_ == ";" && !table_of_functions[function_name].prototype_) {
        table_of_functions[function_name].prototype_ = true;
        gl();
        current_scope = current_scope->BackPreviousScope();
        poliz.SetSize();
        return;
    }
    table_of_functions[function_name].realization_ = true;
    table_of_functions[function_name].adress_of_start_ = adress_of_start_function;
    Block(false);
    //poliz.Push("ADRESS RETURN");
    //poliz.Push("!");
    poliz.SetAdressInBlankSpace(missing_adress_start, poliz.GetCurrentIndex());
}

void Global() {
    if (current_lexeme.type_ != "official word") throw std::string("function decloration was expected in line " + std::to_string(current_lexeme.num_of_string_));
    int index_of_type = index;
    if (current_lexeme.content_ == "void") {
        FunctionDecloration();
        return;
    }
    if (CorrectType(current_lexeme.content_)) {
        gl();
        Identifier(false);
        if (current_lexeme.content_ == "(") {
            gl(index_of_type);
            FunctionDecloration();
            return;
        }
        /*if (current_lexeme.content_ == "=" || current_lexeme.content_ == ";" || current_lexeme.content_ == ",") {
            gl(index_of_type);
            VariableDecloration();
            return;
        }*/
    }
    throw std::string("function decloration was expected in line " + std::to_string(current_lexeme.num_of_string_));
}

void Program() {
    gl();
    while (index != lexemes.size()) {
        Global();
    }
    poliz.Push("function_call_main");
    poliz.Push("end_of_program");
}

void SyntaxAnalyzer(std::vector<Lexeme> lexemes_) {
    swap(lexemes, lexemes_);
    try {
        Program();
        std::cout << "ok" << '\n';
    }
    catch (std::string str) {
        std::cout << str;
    }
}

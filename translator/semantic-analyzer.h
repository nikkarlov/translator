#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "lexeme.h"
Lexeme current_lexeme;

struct TypeNameData
{
    std::string type_, name_, data_;
};

class Scope
{
public:
    Scope() {
        table_of_tnds_.clear();
        childs_.resize(0);
        parent_ = nullptr;
    }
    void PushIndetifier(std::string type, std::string name, std::string value = "") {
        table_of_tnds_[name] = { type, name, value };
    }
    bool IsIdentifierMe(std::string name) {
        return table_of_tnds_.count(name);
    }
    bool IsIdentifierParent(std::string name) {
        return parent_ && parent_->IsIdentifier(name);
    }
    bool IsIdentifier(std::string name) {
        return IsIdentifierMe(name) || IsIdentifierParent(name);
    }
    std::string GetType(std::string identifier) {
        if (!table_of_tnds_.count(identifier)) return parent_->GetType(identifier);
        return table_of_tnds_[identifier].type_;
    }
    Scope* CreateNewScope() {
        Scope* new_scope = new Scope();
        childs_.push_back(new_scope);
        new_scope->parent_ = this;
        return new_scope;
    }
    Scope* BackPreviousScope() {
        return parent_;
    }
private:
    std::map <std::string, TypeNameData> table_of_tnds_;  // table of {type, name, data}
    std::vector<Scope*> childs_;
    Scope* parent_;
};

struct TypeNameParams
{
    std::string type_, name_;
    std::vector<std::string> types_;
};

std::map <std::string, TypeNameParams> table_of_functions;

std::string IsFunction(std::string name, std::vector<std::string> types) {
    if (table_of_functions.count(name) == 0) return "the function " + name + " is not declared ";
    if (table_of_functions[name].types_.size() != types.size()) return "incorrect number of arguments specified ";
    for (int i = 0; i < table_of_functions[name].types_.size(); ++i) {
        if (table_of_functions[name].types_[i] != types[i]) return "the type of the " + std::to_string(i + 1) + " formal parameter does not match the actual one ";
    }
    return "ok";
}

bool addFunction(std::string type, std::string name, std::vector<std::string> types) {
    if (table_of_functions.count(name)) return false;
    table_of_functions[name] = { type, name, types };
    return true;
}

class ControlTypesInExpressions
{
public:
    void Push(std::string string) {
        stack_of_types_and_operations_.push(string);
    }
    void CheckBinary() {
        std::string operand2 = stack_of_types_and_operations_.top();
        stack_of_types_and_operations_.pop();
        std::string operation = stack_of_types_and_operations_.top();
        stack_of_types_and_operations_.pop();
        std::string operand1 = stack_of_types_and_operations_.top();
        stack_of_types_and_operations_.pop();

        if (operand1.substr(0, 4) == "mass") operand1 = operand1.substr(4);
        if (operand2.substr(0, 4) == "mass") operand2 = operand2.substr(4);

        if (operation == "+" || operation == "-" || operation == "*" || operation == "/" || operation == "%") {
            if (operand1 == "int" && operand2 == "int") {
                Push(operand1);
            }
            if (operand1 == "int" && operand2 == "float") {
                Push(operand2);
                if (operation == "%") throw "int and float" + std::to_string(current_lexeme.num_of_string_);
            }
            if (operand1 == "int" && operand2 == "bool") {
                Push(operand1);
            }
            if (operand1 == "int" && operand2 == "char") {
                Push(operand1);
            }
            if (operand1 == "int" && operand2 == "string") {
                //error
            }
            
            if (operand1 == "float" && operand2 == "int") {
                Push(operand1);
                if (operation == "%") throw "int and float" + std::to_string(current_lexeme.num_of_string_);
            }
            if (operand1 == "float" && operand2 == "float") {
                Push(operand1);
                if (operation == "%");//error
            }
            if (operand1 == "float" && operand2 == "bool") {
                Push(operand1);
                if (operation == "%");//error
            }
            if (operand1 == "float" && operand2 == "char") {
                Push(operand1);
                if (operation == "%");//error
            }
            if (operand1 == "float" && operand2 == "string") {
                //error
            }

            if (operand1 == "bool" && operand2 == "int") {
                Push(operand2);
            }
            if (operand1 == "bool" && operand2 == "float") {
                Push(operand2);
                if (operation == "%");//error
            }
            if (operand1 == "bool" && operand2 == "bool") {
                Push(operand1);
            }
            if (operand1 == "bool" && operand2 == "char") {
                Push("int");
            }
            if (operand1 == "bool" && operand2 == "string") {
                //error
            }

            if (operand1 == "char" && operand2 == "int") {
                Push(operand2);
            }
            if (operand1 == "char" && operand2 == "float") {
                Push(operand2);
                if (operation == "%");//error
            }
            if (operand1 == "char" && operand2 == "bool") {
                Push("int");
            }
            if (operand1 == "char" && operand2 == "char") {
                Push("int");
            }
            if (operand1 == "char" && operand2 == "string") {
                
                if (operation == "+") {
                    Push(operand2);
                } else {
                    //error;
                }
            }

            if (operand1 == "string" && operand2 == "int") {
                //error
            }
            if (operand1 == "string" && operand2 == "float") {
                //error
            }
            if (operand1 == "string" && operand2 == "bool") {
                //error
            }
            if (operand1 == "string" && operand2 == "char") {
                if (operation == "+") {
                    Push(operand1);
                }
                else {
                    //error;
                }
            }
            if (operand1 == "string" && operand2 == "string") {
                if (operation == "+") {
                    Push(operand1);
                }
                else {
                    //error;
                }
            }
        }
        if (operation == "<" || operation == ">" || operation == ">=" || operation == "<=" || operation == "==" || operation == "!=") {
            if (operand1 == "int" && operand2 == "int") {
                Push("bool");
            }
            if (operand1 == "int" && operand2 == "float") {
                Push("bool");
            }
            if (operand1 == "int" && operand2 == "bool") {
                Push("bool");
            }
            if (operand1 == "int" && operand2 == "char") {
                Push("bool");
            }
            if (operand1 == "int" && operand2 == "string") {
                //error
            }

            if (operand1 == "float" && operand2 == "int") {
                Push("bool");
            }
            if (operand1 == "float" && operand2 == "float") {
                Push("bool");
            }
            if (operand1 == "float" && operand2 == "bool") {
                Push("bool");
            }
            if (operand1 == "float" && operand2 == "char") {
                Push("bool");
            }
            if (operand1 == "float" && operand2 == "string") {
                //error
            }

            if (operand1 == "bool" && operand2 == "int") {
                Push("bool");
            }
            if (operand1 == "bool" && operand2 == "float") {
                Push("bool");
            }
            if (operand1 == "bool" && operand2 == "bool") {
                Push("bool");
            }
            if (operand1 == "bool" && operand2 == "char") {
                Push("int");
            }
            if (operand1 == "bool" && operand2 == "string") {
                //error
            }

            if (operand1 == "char" && operand2 == "int") {
                Push("bool");
            }
            if (operand1 == "char" && operand2 == "float") {
                Push("bool");
            }
            if (operand1 == "char" && operand2 == "bool") {
                Push("bool");
            }
            if (operand1 == "char" && operand2 == "char") {
                Push("bool");
            }
            if (operand1 == "char" && operand2 == "string") {

                //error
            }

            if (operand1 == "string" && operand2 == "int") {
                //error
            }
            if (operand1 == "string" && operand2 == "float") {
                //error
            }
            if (operand1 == "string" && operand2 == "bool") {
                //error
            }
            if (operand1 == "string" && operand2 == "char") {
                //error
            }
            if (operand1 == "string" && operand2 == "string") {
                Push("bool");
            }
        }
        if (operation == "=" || operation == "+=" || operation == "-=") {
            if (operand1 == "int" && operand2 == "int") {
                Push(operand1);
            }
            if (operand1 == "int" && operand2 == "float") {
                Push(operand1);
            }
            if (operand1 == "int" && operand2 == "bool") {
                Push(operand1);
            }
            if (operand1 == "int" && operand2 == "char") {
                Push(operand1);
            }
            if (operand1 == "int" && operand2 == "string") {
                //error
            }

            if (operand1 == "float" && operand2 == "int") {
                Push(operand2);
            }
            if (operand1 == "float" && operand2 == "float") {
                Push(operand1);
            }
            if (operand1 == "float" && operand2 == "bool") {
                Push(operand1);
            }
            if (operand1 == "float" && operand2 == "char") {
                Push(operand1);
            }
            if (operand1 == "float" && operand2 == "string") {
                //error
            }

            if (operand1 == "bool" && operand2 == "int") {
                Push(operand1);
            }
            if (operand1 == "bool" && operand2 == "float") {
                Push(operand1);
            }
            if (operand1 == "bool" && operand2 == "bool") {
                Push(operand1);
            }
            if (operand1 == "bool" && operand2 == "char") {
                Push(operand1);
            }
            if (operand1 == "bool" && operand2 == "string") {
                //error
            }

            if (operand1 == "char" && operand2 == "int") {
                Push(operand1);
            }
            if (operand1 == "char" && operand2 == "float") {
                Push(operand1);
            }
            if (operand1 == "char" && operand2 == "bool") {
                Push(operand1);
            }
            if (operand1 == "char" && operand2 == "char") {
                Push(operand1);
            }
            if (operand1 == "char" && operand2 == "string") {

                //error
            }

            if (operand1 == "string" && operand2 == "int") {
                //error
            }
            if (operand1 == "string" && operand2 == "float") {
                //error
            }
            if (operand1 == "string" && operand2 == "bool") {
                //error
            }
            if (operand1 == "string" && operand2 == "char") {
                Push(operand1);
            }
            if (operand1 == "string" && operand2 == "string") {
                Push(operand1);
            }
        }
    }
    void CheckUnary() {
        std::string operand = stack_of_types_and_operations_.top();
        stack_of_types_and_operations_.pop();
        std::string operation = stack_of_types_and_operations_.top();
        stack_of_types_and_operations_.pop();

        if (operation == "!" || operation == "~") {
            if (operand == "int") {
                Push("bool");
            }
            if (operand == "float") {
                Push("bool");
            }
            if (operand == "bool") {
                Push("bool");
            }
            if (operand == "char") {
                Push("int");
            }
            if (operand == "string") {
                //error
            }
        }
    }
    std::string GetTypeFromStack() {
        std::string str = stack_of_types_and_operations_.top();
        stack_of_types_and_operations_.pop();
        return str;
    }
private:
    std::stack<std::string> stack_of_types_and_operations_;
};

//(||a == '|' ||a == '&' || a == '=' || a == '>' || a == '<') {
//    return true;
//}
//return false;
//    }
//
//    bool IsDoubleOperation(const std::string& a) {
//        if (a == "+=" || a == "-=" || a == "*=" || a == "/=" || a == "%=" ||
//            a == "&=" || a == "|=" || a == "^=" || a == "~=" ||
//            a == "!=" || a == "==" || a == ">=" || a == "<=" ||
//            a == "&&" || a == "||" || a == "<<" || a == ">>") {
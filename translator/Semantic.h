#pragma once
#include <string>
#include <vector>

class Vertex;

struct Identifier_type_value
{
    std::string identifier_, type_, value_;
};

Vertex* current_scope;

class Vertex
{
public:
    std::vector<Identifier_type_value> table_of_tids_;
    std::vector<Vertex*> childs_;
    Vertex* parent_;

    void PushId(Identifier_type_value& itv) {
        table_of_tids_.push_back(itv);
        itv = {};

    }
    bool CheckId(std::string identifier) {
        for (auto i: table_of_tids_)
        {
            if (i.identifier_ == identifier) return true;
        }
        if (parent_ != nullptr) return parent_->CheckId(identifier);
        return false;
    }
    
    std::string GetType(std::string identifier) {
        for (auto i : table_of_tids_)
        {
            if (i.identifier_ == identifier) return i.type_;
        }
        if (parent_ != nullptr) return parent_->GetType(identifier);
        return "identifier not found";
    }
    /*std::vector<std::string> GetParams(std::string identifier) {
        for (auto i : table_of_tids_)
        {
            if (i.identifier_ == identifier) {
                std::vector<std::string> tmp;
                for (auto j : i.params_) {
                    tmp.push_back(j.type_);
                }
                std::reverse(tmp.begin(), tmp.end());
                return tmp;
            }
        }
        if (parent_ != nullptr) return parent_->GetParams(identifier);
        return {"error"};
    }*/
    void NewScope() {
        Vertex* vot = new Vertex({{}, {}, this});
        childs_.push_back(vot);
        current_scope = vot;
    }
    void Back() {
        current_scope = current_scope->parent_;
    }
};

class Control_types
{
private:
    int isNum(std::string str) {
        if (str == "int") return 1;
        if (str == "ll") return 2;
        if (str == "float") return 3;
        if (str == "ld") return 4;
        if (str == "bool") return 6;
        return 0;
    }
    int isType(std::string str) {
        if (str == "int") return 1;
        if (str == "ll") return 2;
        if (str == "float") return 3;
        if (str == "ld") return 4;
        if (str == "char") return 5;
        if (str == "bool") return 6;
        return 0;
    }
    std::stack<std::string> stack_of_types;
    std::stack<std::string> stack_of_operations;
public:
    void PushTypes(std::string str) {
        stack_of_types.push(str);
    }
    void PushOperations(std::string str) {
        stack_of_operations.push(str);
    }
    std::string PopTypes() {
        std::string tmp = stack_of_types.top();
        stack_of_types.pop();
        return tmp;
    }
    std::string PopOperations() {
        std::string tmp = stack_of_operations.top();
        stack_of_operations.pop();
        return tmp;
    }
    bool CheckBin(std::string operation) {
        std::string first_operand = stack_of_types.top();
        stack_of_types.pop();
        std::string second_operand = stack_of_types.top();
        stack_of_types.pop();

        if (isType(first_operand) * isType(second_operand) == 0) {
            if (isType(first_operand) == 6 || isType(second_operand) == 6) {
                if (operation == "&&" || operation == "||") {
                    stack_of_types.push("bool");
                    return true;
                }
                else
                {
                    return false;
                }
            }
            if (isType(first_operand) == 0 && isType(second_operand) == 0 && (operation == "=" || operation == "+=" ||
                operation == "!=" || operation == "==")) {
                if (operation == "=" || operation == "+=") {
                    stack_of_types.push("string");
                    return true;
                }
                else
                {
                    stack_of_types.push("bool");
                    return true;

                }
            }
            else
            {
                return false;
            }
        }
        else if (isType(first_operand) == 5 || isType(second_operand) == 5) {
            if (isType(first_operand) == 6 || isType(second_operand) == 6) {
                if (operation == "&&" || operation == "||") {
                    stack_of_types.push("bool");
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else if (isType(first_operand) == 3 || isType(second_operand) == 3 ||
                isType(first_operand) == 4 || isType(second_operand) == 4) {
                return false;
            }
            else if (operation == "=" || operation == "+=" || operation == "-=") {
                stack_of_types.push(first_operand);
                return true;
            }
            else if (operation == "+" || operation == "-")
            {
                stack_of_types.push("ll");
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (isType(first_operand) == 6 || isType(second_operand) == 6) {
            if (operation == "||" || operation == "&&" || operation == "=" && isType(first_operand) == 6 && (isType(second_operand) == 6 ||
                isType(second_operand) == 1 || isType(second_operand) == 2)) {
                stack_of_types.push("bool");
                return true;
            }
            else
            {
                return false;
            }
        }
        else if (isType(first_operand) == 3 || isType(first_operand) == 4 || isType(second_operand) == 3 || isType(second_operand) == 4) {
            if (operation == "%" || operation == "%=") {
                return false;
            }
            if (operation == ">" || operation == "<" || operation == ">=" || operation == "<=" ||
                operation == "==" || operation == "!=" || operation == "&&" || operation == "||") {
                stack_of_types.push("bool");
                return true;
            }
            if (isType(first_operand) == 4 || isType(second_operand) == 4 || isType(first_operand) == 2 || isType(second_operand) == 2) {
                stack_of_types.push("ld");
                return true;
            }
            else
            {
                stack_of_types.push("float");
                return true;
            }
        }
        else 
        {
            if (operation == ">" || operation == "<" || operation == ">=" || operation == "<=" ||
                operation == "==" || operation == "!=" || operation == "&&" || operation == "||") {
                stack_of_types.push("bool");
                return true;
            }
            if (isType(first_operand) == 1 && isType(second_operand) == 1) {
                stack_of_types.push("int");
                return true;
            }
            stack_of_types.push("ll");
            return true;
        }
    }
    bool CheckUno(std::string operation) {
        std::string operand = stack_of_types.top();
        stack_of_types.pop();

        if (operation == "+" || operation == "-") {
            if (isNum(operand)) {
                if (operand == "bool") {
                    stack_of_types.push("int");
                    return true;
                }
                stack_of_types.push(operand);
                return true;
            }
            return false;
        }
        stack_of_types.push("bool");
        return true;
    }
    bool isUno(std::string str) {
        if (str == "+" || str == "-" || str == "!") {
            return CheckUno(str);
        }
        return CheckBin(str);
    }
};

class Control_types_in_operations
{
public:
    void Push(std::string string) {
        stack_of_types_and_expressions.push(string);
    }
    void CheckBinary() {
        std::string type_of_1op = stack_of_types_and_expressions.top();
        stack_of_types_and_expressions.pop();
        std::string operation = stack_of_types_and_expressions.top();
        stack_of_types_and_expressions.pop();
        std::string type_of_2op = stack_of_types_and_expressions.top();
        stack_of_types_and_expressions.pop();


    }
private:
    std::stack<std::string> stack_of_types_and_expressions;
};
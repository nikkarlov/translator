#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <deque>
#include <iostream>
#include <fstream>

std::stack<std::string> operands;
std::stack<std::string> adress_variables;

std::stack<int> return_indexes;

std::stack<std::map<std::string, std::vector<std::string>>> variables;

bool next_variable_is_adress = false;
void Reverse(int count) {
    std::deque<std::string> temp_operands;
    for (int i = 0; i < count; i++)
    {
        temp_operands.push_back(operands.top());
        operands.pop();
    }
    for (int i = 0; i < count; i++)
    {
        operands.push(temp_operands.front());
        temp_operands.pop_front();
    }
}
void Launch(std::vector<std::string> poliz_vector) {
    return_indexes.push(284);
    std::ifstream fin("input-data.txt");
    std::ofstream fout("output-data.txt");
    for (int i = 0; i < poliz_vector.size(); i++)
    {
        if (poliz_vector[i] == "!") {
            i = std::atoi(operands.top().c_str()) - 1;
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "!F") {
            int index = std::atoi(operands.top().c_str()) - 1;
            operands.pop();
            if (operands.top() != "true") {
                i = index;
            }
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "=") {
            if (adress_variables.top().substr(0, 5) == "list_") {
                std::string list_name = adress_variables.top().substr(5);
                adress_variables.pop();
                int index = std::atoi(adress_variables.top().c_str());
                adress_variables.pop();
                variables.top()[list_name][index] = operands.top();
                operands.pop();
                continue;
            }

            if (variables.top()[adress_variables.top()].size() == 0) variables.top()[adress_variables.top()].resize(1);
            variables.top()[adress_variables.top()][0] = operands.top();
            adress_variables.pop();
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "-=") {
            if (adress_variables.top().substr(0, 5) == "list_") {
                std::string list_name = adress_variables.top().substr(5);
                adress_variables.pop();
                int index = std::atoi(adress_variables.top().c_str());
                adress_variables.pop();
                variables.top()[list_name][index] = std::to_string(std::atoi(variables.top()[list_name][index].c_str()) - std::atoi(operands.top().c_str()));
                operands.pop();
                continue;
            }

            if (variables.top()[adress_variables.top()].size() == 0) variables.top()[adress_variables.top()].resize(1);
            variables.top()[adress_variables.top()][0] = std::to_string(std::atoi(variables.top()[adress_variables.top()][0].c_str()) - std::atoi(operands.top().c_str()));
            adress_variables.pop();
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "+=") {
            if (adress_variables.top().substr(0, 5) == "list_") {
                std::string list_name = adress_variables.top().substr(5);
                adress_variables.pop();
                int index = std::atoi(adress_variables.top().c_str());
                adress_variables.pop();
                variables.top()[list_name][index] = std::to_string(std::atoi(variables.top()[list_name][index].c_str()) + std::atoi(operands.top().c_str()));
                operands.pop();
                continue;
            }

            if (variables.top()[adress_variables.top()].size() == 0) variables.top()[adress_variables.top()].resize(1);
            variables.top()[adress_variables.top()][0] = std::to_string(std::atoi(variables.top()[adress_variables.top()][0].c_str()) + std::atoi(operands.top().c_str()));
            adress_variables.pop();
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "*=") {
            if (adress_variables.top().substr(0, 5) == "list_") {
                std::string list_name = adress_variables.top().substr(5);
                adress_variables.pop();
                int index = std::atoi(adress_variables.top().c_str());
                adress_variables.pop();
                variables.top()[list_name][index] = std::to_string(std::atoi(variables.top()[list_name][index].c_str()) * std::atoi(operands.top().c_str()));
                operands.pop();
                continue;
            }

            if (variables.top()[adress_variables.top()].size() == 0) variables.top()[adress_variables.top()].resize(1);
            variables.top()[adress_variables.top()][0] = std::to_string(std::atoi(variables.top()[adress_variables.top()][0].c_str()) * std::atoi(operands.top().c_str()));
            adress_variables.pop();
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "/=") {
            if (adress_variables.top().substr(0, 5) == "list_") {
                std::string list_name = adress_variables.top().substr(5);
                adress_variables.pop();
                int index = std::atoi(adress_variables.top().c_str());
                adress_variables.pop();
                variables.top()[list_name][index] = std::to_string(std::atoi(variables.top()[list_name][index].c_str()) / std::atoi(operands.top().c_str()));
                operands.pop();
                continue;
            }

            if (variables.top()[adress_variables.top()].size() == 0) variables.top()[adress_variables.top()].resize(1);
            variables.top()[adress_variables.top()][0] = std::to_string(std::atoi(variables.top()[adress_variables.top()][0].c_str()) / std::atoi(operands.top().c_str()));
            adress_variables.pop();
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "%=") {
            if (adress_variables.top().substr(0, 5) == "list_") {
                std::string list_name = adress_variables.top().substr(5);
                adress_variables.pop();
                int index = std::atoi(adress_variables.top().c_str());
                adress_variables.pop();
                variables.top()[list_name][index] = std::to_string(std::atoi(variables.top()[list_name][index].c_str()) % std::atoi(operands.top().c_str()));
                operands.pop();
                continue;
            }

            if (variables.top()[adress_variables.top()].size() == 0) variables.top()[adress_variables.top()].resize(1);
            variables.top()[adress_variables.top()][0] = std::to_string(std::atoi(variables.top()[adress_variables.top()][0].c_str()) % std::atoi(operands.top().c_str()));
            adress_variables.pop();
            operands.pop();
            continue;
        }
        if (poliz_vector[i] == "+") {
            auto operand1 = std::atof(operands.top().c_str());
            operands.pop();
            auto operand2 = std::atof(operands.top().c_str());
            operands.pop();

            operands.push(std::to_string(operand1 + operand2));
            continue;
        }
        if (poliz_vector[i] == "-") {
            auto operand1 = std::stoi(operands.top().c_str());
            operands.pop();
            auto operand2 = std::stoi(operands.top().c_str());
            operands.pop();

            operands.push(std::to_string(operand2 - operand1));
            continue;
        }
        if (poliz_vector[i] == "*") {
            auto operand1 = std::stoi(operands.top().c_str());
            operands.pop();
            auto operand2 = std::stoi(operands.top().c_str());
            operands.pop();

            operands.push(std::to_string(operand1 * operand2));
            continue;
        }
        if (poliz_vector[i] == "/") {
            auto operand1 = std::stoi(operands.top().c_str());
            operands.pop();
            auto operand2 = std::stoi(operands.top().c_str());
            operands.pop();

            operands.push(std::to_string(operand2 / operand1));
            continue;
        }
        if (poliz_vector[i] == "%") {
            auto operand1 = std::stoi(operands.top().c_str());
            operands.pop();
            auto operand2 = std::stoi(operands.top().c_str());
            operands.pop();

            operands.push(std::to_string(operand2 % operand1));
            continue;
        }
        if (poliz_vector[i] == "unary_-") {
            auto operand1 = std::stoi(operands.top().c_str());
            operands.pop();
            operands.push(std::to_string(-operand1));
            continue;
        }
        if (poliz_vector[i] == "unary_+") {
            continue;
        }
        if (poliz_vector[i].substr(0, 14) == "function_call_") {
            return_indexes.push(i);
            variables.push({});
            Reverse(table_of_functions[poliz_vector[i].substr(14)].types_of_params_.size());
            i = table_of_functions[poliz_vector[i].substr(14)].adress_of_start_ - 1;
            continue;
        }
        if (next_variable_is_adress == true) {
            adress_variables.push(poliz_vector[i].substr(9));
            next_variable_is_adress = false;
            continue;
        }
        if (poliz_vector[i].substr(0, 9) == "variable_") {
            if (poliz_vector[i + 1] == "adress") {
                adress_variables.push(poliz_vector[i].substr(9));
                i++;
            }
            else
            {
                if (!variables.top().count(poliz_vector[i].substr(9))) {
                    std::cout << "var is not assign";
                    return;
                }
                operands.push(variables.top()[poliz_vector[i].substr(9)][0]);
            }
            continue;
        }
        if (poliz_vector[i] == "list_size") {
            std::string list_name = operands.top();
            operands.pop();
            int list_size = std::atoi(operands.top().c_str());
            operands.pop();
            variables.top()[list_name].resize(list_size);
            continue;
        }
        if (poliz_vector[i] == "INPUT") {
            std::string input_data;
            fin >> input_data;
            operands.push(input_data);
            continue;
        }
        if (poliz_vector[i] == "[]") {
            std::string list_name = operands.top();
            operands.pop();
            int index = std::atoi(operands.top().c_str());
            operands.pop();
            if (index >= variables.top()[list_name].size() || index < 0) {
                std::cout << "incorrect index";
                return;
            }
            if (poliz_vector[i + 1] == "adress") {
                adress_variables.push(std::to_string(index));
                adress_variables.push("list_" + list_name);
                i++;
            }
            else
            {
                if (variables.top()[list_name][index] == "") {
                    std::cout << "do not declared";
                    return;
                }
                operands.push(variables.top()[list_name][index]);
            }
            continue;
        }
        if (poliz_vector[i] == "print") {
            int cnt = std::atoi(operands.top().c_str());
            operands.pop();
            std::vector<std::string> str;
            for (int i = 0; i < cnt; i++)
            {
                str.push_back("\n");
                str.push_back(operands.top());
                operands.pop();
            }
            std::reverse(str.begin(), str.end());
            for (auto i : str)
            {
                fout << i;
            }
            continue;
        }
        if (poliz_vector[i] == "ADRESS POLIZ") {
            continue;
        }
        if (poliz_vector[i] == "==") {
            auto operand1 = operands.top();
            operands.pop();
            auto operand2 = operands.top();
            operands.pop();

            if (operand2 == operand1) {
                operands.push("true");
            }
            else
            {
                operands.push("false");
            }
            continue;
        }
        if (poliz_vector[i] == ">=") {
            auto operand1 = operands.top();
            operands.pop();
            auto operand2 = operands.top();
            operands.pop();

            if (std::stoi(operand2.c_str()) >= std::stoi(operand1.c_str())) {
                operands.push("true");
            }
            else
            {
                operands.push("false");
            }
            continue;
        }
        if (poliz_vector[i] == "<=") {
            auto operand1 = operands.top();
            operands.pop();
            auto operand2 = operands.top();
            operands.pop();

            if (std::stoi(operand2.c_str()) <= std::stoi(operand1.c_str())) {
                operands.push("true");
            }
            else
            {
                operands.push("false");
            }
            continue;
        }
        if (poliz_vector[i] == "<") {
            auto operand1 = operands.top();
            operands.pop();
            auto operand2 = operands.top();
            operands.pop();

            if (std::stoi(operand2.c_str()) < std::stoi(operand1.c_str())) {
                operands.push("true");
            }
            else
            {
                operands.push("false");
            }
            continue;
        }
        if (poliz_vector[i] == ">") {
            auto operand1 = operands.top();
            operands.pop();
            auto operand2 = operands.top();
            operands.pop();

            if (std::stoi(operand2.c_str()) > std::stoi(operand1.c_str())) {
                operands.push("true");
            }
            else
            {
                operands.push("false");
            }
            continue;
        }
        if (poliz_vector[i] == "ADRESS VOZVRATA") {
            i = return_indexes.top();
            return_indexes.pop();
            variables.pop();
            continue;
        }
        if (poliz_vector[i] == "end_of_program") {
            std::cout << "OK";
            return;
        }
        

        operands.push(poliz_vector[i]);
    }
    fin.close();
    fout.close();
}
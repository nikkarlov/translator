#pragma once
#include <string>

struct Lexeme
{
    Lexeme() {};
    Lexeme(std::string a, std::string b, int c) : content_(a), type_(b), num_of_string_(c) {};
    std::string content_, type_;
    int num_of_string_;
};
#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include "lexeme.h"

int is_push_poliz = 1;

class PolizGenerator
{
public:
    void PolizPush(std::string str) {
        if (is_push_poliz == 1) poliz_.push_back(str);
    }
private:
    std::vector<std::string> poliz_;
};

PolizGenerator poliz;
#pragma once
#include <string>
#include <vector>
#include <map>
#include <stack>
#include <iostream>
#include "lexeme.h"

class PolizGenerator
{
public:
    void Push(std::string str) {
        if (is_push_poliz_ == 1) poliz_.push_back(str);
    }
    void Disable() {
        is_push_poliz_--;
    }
    void Enable() {
        is_push_poliz_++;
    }
    void SetPossibleSize() {
        possible_size_ = poliz_.size();
    }
    void SetSize() {
        poliz_.resize(possible_size_);
    }
    void Print() {
        for (int i = 0; i < poliz_.size(); i++)
        {
            std::cout << i << ". " << poliz_[i] << '\n';
        }
    }
    void SetBlankSpace() {
        poliz_.push_back("");
        return;
    }
    void SetAdressInBlankSpace(int index, int value) {
        poliz_[index] = std::to_string(value);
        return;
    }
    int GetCurrentIndex() {
        return poliz_.size();
    }
    std::vector<std::string> GetPoliz() {
        return poliz_;
    }
private:
    std::vector<std::string> poliz_;
    int is_push_poliz_ = 1;
    int possible_size_;
};

PolizGenerator poliz;
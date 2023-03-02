#pragma once
#include <string>
#include <vector>

std::stack<std::string> types_and_operations;


//void Push(std::string str) {
//    types_and_operations.push(str);
//}
//std::string makeOperation() {
//
//}
//void CheckBin() {
//    std::string first_operator = types_and_operations.top();
//    types_and_operations.pop();
//
//    std::string operation = types_and_operations.top();
//    types_and_operations.pop();
//
//    std::string second_operator = types_and_operations.top();
//    types_and_operations.pop();
//
//    types_and_operations.push(makeOperation());
//}
//void CheckUno() {
//    std::string first_operator = types_and_operations.top();
//    types_and_operations.pop();
//
//    std::string operation = types_and_operations.top();
//    types_and_operations.pop();
//
//    std::string second_operator = types_and_operations.top();
//    types_and_operations.pop();
//
//    types_and_operations.push(makeOperation());
//}

class Vertex_of_tree;

struct Itv
{
    std::string identifier_, type_;
    std::vector<Itv> params_;
};


Vertex_of_tree* current_scope;

class Vertex_of_tree
{
public:
    std::vector<Itv> table_of_tids_;
    std::vector<Vertex_of_tree*> childs_;
    Vertex_of_tree* parent_;

    void PushId(Itv& itv) {
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
    void NewScope() {
        Vertex_of_tree* vot = new Vertex_of_tree({ {},{},this });
        childs_.push_back(vot);
        current_scope = vot;
    }
    void Back() {
        current_scope = current_scope->parent_;
    }
};



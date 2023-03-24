#pragma once
#include "Lexeme.h"
#include <fstream>
#include <set>
#include <vector>

class LexicalAnalyzer
{
public:
    std::vector<Lexeme> LexicalAnalysis() {
        std::ifstream fin("official-words.txt");
        std::string line;
        std::set<std::string> official_words;
        while (getline(fin, line)) {
            official_words.insert(line);
        }
        fin.close();
        fin.open("input-file.txt"); 
        std::string program_text;
        while (getline(fin, line)) {
            program_text += line;
            program_text += '\n';
        }
        fin.close();
        int num_of_string = 1;
        for (int i = 0; i < program_text.size(); i++) {
            if (i + 1 < program_text.size() && program_text[i] == '/' && program_text[i + 1] == '/') {
                while (i < program_text.size() && program_text[i] != '\n') {
                    i++;
                }
                i--;
                continue;
            }
            if (IsLetter(program_text[i])) {
                std::string str;
                while (i < program_text.size() && IsNumAndLetAndUnderscore(program_text[i])) {
                    str += program_text[i];
                    i++;
                }
                if (official_words.find(str) != official_words.end()) {
                    vector_of_lexemes_.push_back({ str, "official word", num_of_string });
                } else {
                    vector_of_lexemes_.push_back({ str, "identifier", num_of_string });
                }
                i--;
                continue;
            }
            if (IsNumber(program_text[i])) {
                std::string str;
                bool flag_dot = true;
                while (i < program_text.size() && (IsNumber(program_text[i]) || (program_text[i] == '.' && flag_dot))) {
                    if (program_text[i] == '.') {
                        flag_dot = false;
                    }
                    str += program_text[i];
                    i++;
                }
                if (flag_dot) {
                    vector_of_lexemes_.push_back({ str, "int numeric literal", num_of_string });
                }
                else {
                    vector_of_lexemes_.push_back({ str, "float numeric literal", num_of_string });
                }
                i--;
                continue;
            }
            if (IsOperation(program_text[i])) {
                if (i + 1 < program_text.size() && IsOperation(program_text[i + 1])) {
                    if (i + 2 < program_text.size() && IsOperation(program_text[i + 2])) {
                        std::string str;
                        str += program_text[i], str += program_text[i + 1], str += program_text[i + 2];
                        if (IsTripleOperation(str)) {
                            vector_of_lexemes_.push_back({ str, "operation", num_of_string });
                            i++;
                            continue;
                        }
                    }
                    std::string str;
                    str += program_text[i], str += program_text[i + 1];
                    if (IsDoubleOperation(str)) {
                        vector_of_lexemes_.push_back({ str, "operation", num_of_string });
                        i++;
                        continue;
                    }
                }
                vector_of_lexemes_.push_back({ std::string(1, program_text[i]), "operation", num_of_string });
                continue;
            }
            if (IsPunctuation(program_text[i])) {
                vector_of_lexemes_.push_back({ std::string(1, program_text[i]), "punctuation", num_of_string });
                continue;
            }
            if (program_text[i] == '(' || program_text[i] == ')') {
                vector_of_lexemes_.push_back({ std::string(1, program_text[i]), "round brackets", num_of_string });
                continue;
            }
            if (program_text[i] == '[' || program_text[i] == ']') {
                vector_of_lexemes_.push_back({ std::string(1, program_text[i]), "square brackets", num_of_string });
                continue;
            }
            if (program_text[i] == '"') {
                std::string str;
                i++;
                while (i < program_text.size() && (program_text[i] != '"')) {
                    str += program_text[i];
                    i++;
                }
                vector_of_lexemes_.push_back({ str, "string literal", num_of_string });
                continue;
            }
            if (program_text[i] == '\'') {
                std::string str;
                i++;
                if (i < program_text.size() && (program_text[i] != '\'')) {
                    str += program_text[i];
                    i++;
                }
                vector_of_lexemes_.push_back({ str, "char literal", num_of_string });
                continue;
            }
            if (program_text[i] == ' ' || program_text[i] == '\t' || program_text[i] == '\n') {
                if (program_text[i] == '\n') num_of_string++;
                continue;
            }
            vector_of_lexemes_.push_back({ std::string(1, program_text[i]), "incorrect symbol", num_of_string });
        }
        return vector_of_lexemes_;
    }
private:
    std::vector<Lexeme> vector_of_lexemes_;
    bool IsLetter(const char& a) {
        if ((a - 'a' >= 0 && a - 'a' < 26) || (a - 'A' >= 0 && a - 'A' < 26)) {
            return true;
        }
        return false;
    }

    bool IsNumber(const char& a) {
        if (a - '0' >= 0 && a - '0' <= 9) {
            return true;
        }
        return false;
    }

    bool IsNumAndLetAndUnderscore(const char& a) {
        if (a == '_') {
            return true;
        }
        return (IsLetter(a) || IsNumber(a));
    }

    bool IsPunctuation(const char& a) {
        if (a == ';' || a == ',' || a == '{' || a == '}') {
            return true;
        }
        return false;
    }

    bool IsOperation(const char& a) {
        if (a == '+' || a == '-' || a == '*' || a == '/' || a == '%' || a == '|' ||
            a == '&' || a == '!' || a == '~' || a == '=' || a == '>' || a == '<') {
            return true;
        }
        return false;
    }

    bool IsDoubleOperation(const std::string& a) {
        if (a == "+=" || a == "-=" || a == "*=" || a == "/=" || a == "%=" ||
            a == "&=" || a == "|=" || a == "^=" || a == "~=" ||
            a == "!=" || a == "==" || a == ">=" || a == "<=" ||
            a == "&&" || a == "||" || a == "<<" || a == ">>") {
            return true;
        }
        return false;
    }

    bool IsTripleOperation(const std::string& a) {
        if (a == "<<=" || a == ">>=") {
            return true;
        }
        return false;
    }
};
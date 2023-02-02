#pragma once
#include "Lexeme.h"
#include <fstream>
#include <set>
#include <vector>
bool Letter(const char& a) {
	if ((a - 'a' >= 0 && a - 'a' < 26) || (a - 'A' >= 0 && a - 'A' < 26)) {
		return true;
	}
	return false;
}
bool Number(const char& a) {
	if (a - '0' >= 0 && a - '0' <= 9) {
		return true;
	}
	return false;
}
bool NumAndLetAndUnderscore(const char& a) {
	if (a == '_') {
		return true;
	}
	return (Letter(a) || Number(a));
}
bool Punctuation(const char& a) {
	if (a == ':' || a == ';' || a == ',' || a == '{' || a == '}') {
		return true;
	}
	return false;
}
bool Operation(const char& a) {
	if (a == '+' || a == '-' || a == '*' || a == '/' || a == '%' ||
		a == '&' || a == '!' || a == '~' || a == '=' || a == '>' || a == '<') {
		return true;
	}
	return false;
}
bool DoubleOperation(const std::string& a) {
	if (a == "+= " || a == "-=" || a == "*=" || a == "/=" || a == "%=" || a == "&=" ||
		a == "!=" || a == "~=" || a == "==" || a == ">=" || a == "<=" ||
		a == "&&" || a == "||" || a == "<<" || a == ">>") {
		return true;
	}
	return false;
}
std::vector<Lexeme> Analiz(bool& fl) {
	fl = 1;
	std::ifstream fin("officialWords.txt");
	std::string line;
	std::set<std::string> ofWords;
	while (getline(fin, line)) {
		ofWords.insert(line);
	}
	fin.close();
	fin.open("input.txt");
	std::string program;
	while (getline(fin, line)) {
		program += line;
		program += '\n';
	}
	fin.close();
	std::vector<Lexeme> vec;
	for (int i = 0; i < program.size(); i++) {
		if (i + 1 < program.size() && program[i] == '/' && program[i + 1] == '/') {
			while (i < program.size() && program[i] != '\n') {
				i++;
			}
			i--;
			continue;
		}
		if (Letter(program[i])) {
			std::string str;
			while (i < program.size() && NumAndLetAndUnderscore(program[i])) {
				str += program[i];
				i++;
			}
			if (ofWords.find(str) != ofWords.end()) {
				vec.push_back({str, "offcial word"});
			}
			else {
				vec.push_back({ str, "identifier" });
			}
			i--;
			continue;
		}
		if (Number(program[i])) {
			std::string str;
			bool flDot = true;
			while (i < program.size() && (Number(program[i]) || (program[i] == '.' && flDot))) {
				if (program[i] == '.') {
					flDot = false;
				}
				str += program[i];
				i++;
			}
			if (i < program.size()) {
				if (Letter(program[i])) {
					fl = 0;
				}
			}
			if (flDot) {
				vec.push_back({ str, "numeric ld literal" });
			}
			else {
				vec.push_back({ str, "numeric ll literal" });
			}
			i--;
			continue;
		}
		if (Operation(program[i])) {
			if (i + 1 < program.size() && Operation(program[i + 1])) {
				std::string str;
				str += program[i], str += program[i + 1];
				if (DoubleOperation(str)) {
					vec.push_back({ str, "operation" });
					i++;
					continue;
				}
			}
			vec.push_back({ std::string(1, program[i]), "operation" });
			continue;
		}
		if (Punctuation(program[i])) {
			vec.push_back({ std::string (1, program[i]), "punctuation"});
			continue;
		}
		if (program[i] == '(' || program[i] == ')') {
			vec.push_back({ std::string(1, program[i]), "round brackets" });
			continue;
		}
		if (program[i] == '[' || program[i] == ']') {
			vec.push_back({ std::string(1, program[i]), "square brackets" });
			continue;
		}
		if (program[i] == '"') {
			std::string str;
			i++;
			while (i < program.size() && (program[i] != '"')) {
				str += program[i];
				i++;
			}
			vec.push_back({ str, "string literal" });
			continue;
		}
		if (program[i] == ' ' || program[i] == '\t' || program[i] == '\n') {
			continue;
		}
		fl = 0;
	}
	return vec;
}
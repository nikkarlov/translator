#pragma once
#include <vector>
#include "Lexeme.h";
Lexeme c;
int i = 0;
std::vector<Lexeme> lexemes;
bool Type_() {

}
void gl() {
	if (i > lexemes.size()) {
		throw "end";
	}
	if (i == lexemes.size()) {
		c.type_ = "end";
	}
	else {
		c = lexemes[i];
		i++;
	}
}
bool String() {
	if (c.content_ == "string") {
		return true;
	}
	return false;
}
bool Array() {
	if (c.content_ == "array") {
		gl();
		if (c.content_ != "<") {
			throw;
		}
		gl();
		Type();
		if (c.content_ != ">") {
			throw;
		}
		gl();
		if (c.content_ != "[") {
			throw;
		}
		gl();
		//Exp();
		if (c.content_ != "]") {
			throw;
		}
		return true;
	}
	return false;
}
bool CharType() {
	if (c.content_ == "char") {
		return true;
	}
	return false;
}
bool ArrayType() {
	if (Array() || String()) {
		return true;
	}
	return false;
}
bool NumType() {
	if (c.content_ == "int" || c.content_ == "ll" || c.content_ == "float" || c.content_ == "ld") {
		return true;
	}
	return false;
}
void Type() {
	if (!(NumType() || ArrayType() || CharType())) {
		throw;
	}
	gl();
}
void Parameters() {
	if (c.content_ != ")") {
		Type();
		gl();
		if (c.type_ != "identifier") {
			throw;
		}
		gl();
	}
	while (c.content_ == ",") {
		Type();
		gl();
		if (c.type_ != "identifier") {
			throw;
		}
		gl();
	}
}
void Block() {
	if (c.content_ != "{") {
		throw;
	}
	gl();
}
void Function() {
	Type();
	bool fl_main = 0;
	if (c.content_ == "main") {
		fl_main;
	}
	else {
		if (c.type_ != "identifier") {
			throw;
		}
	}
	gl();
	if (c.content_ != "(") {
		throw;
	}
	gl();
	Parameters();
	if (c.content_ != ")") {
		throw;
	}
	gl();
	Block();
	if (!fl_main) {
		Function();
	}
}
void Program() {
	gl();
}

std::string SyntaxAnalyzer(std::vector<Lexeme> lexemes_) {
	swap(lexemes, lexemes_);
}
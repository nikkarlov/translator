#pragma once
#include <iostream>
#include <vector>
#include "Lexeme.h";
Lexeme c;
int i = 0;
std::vector<Lexeme> lexemes;

void gl();
void Exp();
void Type();
void Block();
void Exp8();

bool Type_() {
	if (c.content_ == "int" || c.content_ == "ll" || c.content_ == "float" || c.content_ == "ld" ||
		c.content_ == "string" || c.content_ == "char" || c.content_ == "array") {
		return true;
	}
	return false;
}
bool Prior1() {
	if (c.content_ == "++" || c.content_ == "--") {
		return true;
	}
	if (c.content_ == "[") {
		gl();
		Exp();
		if (c.content_ != "]") {
			throw;
		}
		return true;
	}
	if (c.content_ == "(") {
		gl();
		Exp();
		if (c.content_ != ")") {
			throw;
		}
		return true;
	}
	return false;
}
bool Prior2() {
	if (c.content_ == "++" || c.content_ == "--" || c.content_ == "+" || c.content_ == "-" ||
		c.content_ == "~" || c.content_ == "!") {
		return true;
	}
	return false;
}
bool Prior3() {
	if (c.content_ == "*" || c.content_ == "/" || c.content_ == "%") {
		return true;
	}
	return false;
}
bool Prior4() {
	if (c.content_ == "+" || c.content_ == "-") {
		return true;
	}
	return false;
}
bool Prior5() {
	if (c.content_ == ">" || c.content_ == "<" || c.content_ == ">=" || c.content_ == "<=") {
		return true;
	}
	return false;
}
bool Prior6() {
	if (c.content_ == "==" || c.content_ == "!=") {
		return true;
	}
	return false;
}
bool Prior7() {
	if (c.content_ == "&&") {
		return true;
	}
	return false;
}
bool Prior8() {
	if (c.content_ == "||") {
		return true;
	}
	return false;
}
bool Prior9() {
	if (c.content_ == "=" || c.content_ == "+=" || c.content_ == "-=" || c.content_ == "*=" ||
		c.content_ == "/=" || c.content_ == "%=") {
		return true;
	}
	return false;
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
	}
	i++;
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
		Exp();
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
		if (c.type_ != "identifier") {
			throw;
		}
		gl();
	}
	while (c.content_ == ",") {
		gl();
		Type();
		if (c.type_ != "identifier") {
			throw;
		}
		gl();
	}
}
void Exp1() {
	if (c.type_ == "numeric literal") {
		gl();
	}
	else if (c.type_ == "identifier") {
		gl();
		if (Prior1()) {
			gl();
		}
	}
	else if (c.content_ != "(") {
		throw;
	}
	else {
		gl();
		Exp8();
		if (c.content_ != ")") {
			throw;
		}
		gl();
	}
}
void Exp2() {
	if (Prior2()) {
		gl();
	}
	Exp1();
}
void Exp3() {
	Exp2();
	while (Prior3()) {
		gl();
		Exp2();
	}
}
void Exp4() {
	Exp3();
	while (Prior4()) {
		gl();
		Exp3();
	}
}
void Exp5() {
	Exp4();
	while (Prior5()) {
		gl();
		Exp4();
	}
}
void Exp6() {
	Exp5();
	while (Prior6()) {
		gl();
		Exp5();
	}
}
void Exp7() {
	Exp6();
	while (Prior7()) {
		gl();
		Exp6();
	}
}
void Exp8() {
	Exp7();
	while (Prior8()) {
		gl();
		Exp7();
	}
}
void Exp() {
	Exp8();
}
void Lvalue() {
	if (c.type_ != "identifier") {
		throw;
	}
	gl();
	if (c.content_ == "[") {
		gl();
		Exp();
		if (c.content_ != "]") {
			throw;
		}
		gl();
	}
}
void Exp9() {
	Lvalue();
	if (!Prior9()) {
		throw;
	}
	gl();
	Exp8();
}
void While() {
	if (c.content_ != "(") {
		throw;
	}
	gl();
	Exp();
	if (c.content_ != ")") {
		throw;
	}
	gl();
	Block();
}
void For() {
	if (c.content_ != "(") {
		throw;
	}
	gl();
	if (Type_()) {
		gl();
		if (c.type_ != "identifier") {
			throw;
		}
		gl();
		if (c.content_ == "=") {
			gl();
			if (c.type_ == "string literal") {
				gl();
			}
			else {
				Exp();
			}
		}
		while (c.content_ == ",") {
			gl();
			if (c.type_ != "identifier") {
				throw;
			}
			gl();
			if (c.content_ == "=") {
				gl();
				if (c.type_ == "string literal") {
					gl();
				}
				else {
					Exp();
				}
			}
		}
		if (c.content_ != ";") {
			throw;
		}
	}
	else if (c.type_ == "identifier") {
		if (c.type_ != "identifier") {
			throw;
		}
		gl();
		if (c.content_ == "=") {
			gl();
			if (c.type_ == "string literal") {
				gl();
			}
			else {
				Exp();
			}
		}
		while (c.content_ == ",") {
			gl();
			if (c.type_ != "identifier") {
				throw;
			}
			gl();
			if (c.content_ == "=") {
				gl();
				if (c.type_ == "string literal") {
					gl();
				}
				else {
					Exp();
				}
			}
		}
		if (c.content_ != ";") {
			throw;
		}
	}
	else if (c.content_ == ";") {
		gl();
	}
	else {
		throw;
	}
	Exp();
	if (c.content_ != ";") {
		throw;
	}
	gl();
	Exp();
	if (c.content_ != ")") {
		throw;
	}
	gl();
	Block();
}
void If() {
	if (c.content_ != "(") {
		throw;
	}
	gl();
	Exp();
	if (c.content_ != ")") {
		throw;
	}
	gl();
	Block();
	if (c.content_ == "else") {
		gl();
		Block();
	}
}
void Return() {
	Exp();
}
void Cout() {
	if (c.content_ != "<<") {
		throw "r";
	}
	gl();
	if (c.type_ == "string literal") {
		gl();
	}
	else {
		Exp();
	}
	while (c.content_ == "<<") {
		gl();
		if (c.type_ == "string literal") {
			gl();
		}
		else {
			Exp();
		}
	}
}
void Cin() {
	if (c.content_ != ">>") {
		throw "r";
	}
	gl();
	if (c.type_ != "identifier") {
		throw;
	}
	gl();
	while (c.content_ == ">>") {
		gl();
		if (c.type_ != "identifier") {
			throw "r";;
		}
		gl();
	}
}
void Operator() {
	if (c.content_ == "cin") {
		gl();
		Cin();
		if (c.content_ != ";") {
			throw "r";
		}
		gl();
	}
	else if (c.content_ == "cout"){
		gl();
		Cout();
		if (c.content_ != ";") {
			throw "r";
		}
		gl();
	}
	else if (c.content_ == "while") {
		gl();
		While();
	}
	else if (c.content_ == "for") {
		gl();
		For();
	}
	else if (c.content_ == "if") {
		gl();
		If();
	}
	else if (c.content_ == "return") {
		gl();
		Return();
		if (c.content_ != ";") {
			throw "r";;
		}
		gl();
	}
	else {
		Exp;
		if (c.content_ != ";") {
			throw "r";;
		}
		gl();
	}
}
void Block() {
	if (c.content_ != "{") {
		throw "r";;
	}
	gl();
	while (c.content_ != "}") {
		if (Type_()) {
			gl();
			if (c.type_ != "identifier") {
				throw;
			}
			gl();
			if (c.content_ == "=") {
				gl();
				if (c.type_ == "string literal") {
					gl();
				}
				else {
					Exp();
				}
			}
			while (c.content_ == ",") {
				gl();
				if (c.type_ != "identifier") {
					throw;
				}
				gl();
				if (c.type_ == "string literal") {
					gl();
				}
				else {
					Exp();
				}
			}
			if (c.content_ != ";") {
				throw;
			}
			gl();
		}
		else if (c.type_ == "identifier") {
			Exp9();
		}
		else {
			Operator();
		}
	}
	if (c.content_ != "}") {
		throw;
	}
	gl();
}
void Function() {
	Type();
	bool fl_main = 0;
	if (c.content_ == "main") {
		fl_main = 1;
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
	Function();
	if (c.type_ != "end") {
		throw "lishnie symbol";
	}
}

void SyntaxAnalyzer(std::vector<Lexeme> lexemes_) {
	swap(lexemes, lexemes_);
	try {
		Program();
		if (i < lexemes.size()) {
			throw;
		}
	}
	catch (const char str[]) {
		std::cout << "err";
	}
}
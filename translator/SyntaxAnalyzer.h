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
void Global();

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
			throw std::string("expected to get a symbol ] in line " + std::to_string(c.num_of_string_));
		}
		return true;
	}
	if (c.content_ == "(") {
		gl();
		Exp();
		if (c.content_ != ")") {
			throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
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
		throw std::string("end");
	}
	if (i == lexemes.size()) {
		c.type_ = "end";
	}
	else {
		c = lexemes[i];
        if (c.type_ == "incorrect symbol") {
            throw std::string("incorrect symbol in line " + std::to_string(c.num_of_string_));
        }
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
			throw std::string("expected to get a symbol < in line " + std::to_string(c.num_of_string_));
		}
		gl();
		Type();
		if (c.content_ != ">") {
			throw std::string("expected to get a symbol > in line " + std::to_string(c.num_of_string_));
		}
		gl();
		if (c.content_ != "[") {
			throw std::string("expected to get a symbol [ in line " + std::to_string(c.num_of_string_));
		}
		gl();
		Exp();
		if (c.content_ != "]") {
			throw std::string("expected to get a symbol ] in line " + std::to_string(c.num_of_string_));
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
		throw std::string("invalid data type in line " + std::to_string(c.num_of_string_));
	}
	gl();
}
void FunctionType() {
    if (!(NumType() || ArrayType() || CharType() || c.content_ == "void")) {
        throw std::string("invalid data type in line " + std::to_string(c.num_of_string_));
    }
    gl();
}
void Parameters() {
	if (c.content_ != ")") {
		Type();
		if (c.type_ != "identifier") {
			throw std::string("r");
		}
		gl();
	}
	while (c.content_ == ",") {
		gl();
		Type();
		if (c.type_ != "identifier") {
			throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
		}
		gl();
	}
}
void Enumeration() {
	if (c.content_ != ")") {
		if (c.type_ == "string literal") {
			gl();
		}
		else {
			Exp();
		}
	}
	while (c.content_ == ",") {
		gl();
		if (c.type_ == "string literal") {
			gl();
		}
		else {
			Exp();
		}
	}
}
void Exp1() {
	if (c.type_ == "numeric ll literal" || c.type_ == "numeric ld literal") {
		gl();
	}
	else if (c.type_ == "identifier") {
		gl();
		if (c.content_ == "(") {
			gl();
			Enumeration();
			if (c.content_ != ")") {
				throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
			}
			gl();
		}
		else if (Prior1()) {
			gl();
			if (Prior1()) {
				gl();
			}
		}
	}
	else if (c.content_ != "(") {
        throw std::string("expected to get a expression in line " + std::to_string(c.num_of_string_));
	}
	else {
		gl();
		Exp8();
		if (c.content_ != ")") {
            throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
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
		throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
	}
	gl();
	if (c.content_ == "[") {
		gl();
		Exp();
		if (c.content_ != "]") {
            throw std::string("expected to get a symbol ] in line " + std::to_string(c.num_of_string_));
		}
		gl();
	}
}
void Exp9() {
	Lvalue();
	if (!Prior9()) {
        throw std::string("expected assignment in line " + std::to_string(c.num_of_string_));
	}
	if (c.content_ == "=") {
		gl();
		if (c.type_ == "string literal") {
			gl();
		}
		else {
			Exp8();
		}
	}
	else {
		gl();
		Exp8();
	}
}
void While() {
	if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Exp();
	if (c.content_ != ")") {
        throw std::string("expected to get a symbol ] ) in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Block();
}
void For() {
	if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
	}
	gl();
	if (Type_()) {
		gl();
		if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
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
                throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
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
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
		}
	}
	else if (c.type_ == "identifier") {
		if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
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
                throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
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
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
		}
	}
	if (c.content_ == ";") {
		gl();
	}
	else {
        throw std::string("incorrect cycle definition in line " + std::to_string(c.num_of_string_));
	}
	Exp();
	if (c.content_ != ";") {
        throw std::string(" expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Exp();
	if (c.content_ != ")") {
        throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Block();
}
void If() {
	if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Exp();
	if (c.content_ != ")") {
        throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Block();
	if (c.content_ == "else") {
		gl();
		Block();
	}
}
void Return() {
	if (c.type_ == "string literal") {
		gl();
	}
    else if (c.content_ != ";") {
        Exp();
    }
}
void Cout() {
	if (c.content_ != "<<") {
        throw std::string("expected to get a symbols << in line " + std::to_string(c.num_of_string_));
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
        throw std::string("expected to get a symbols >> in line " + std::to_string(c.num_of_string_));
	}
	gl();
	if (c.type_ != "identifier") {
        throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
	}
	gl();
	while (c.content_ == ">>") {
		gl();
		if (c.type_ != "identifier") {
            throw std::string("variable declaration was expected in line " + std::to_string(c.num_of_string_));
		}
		gl();
	}
}
void Declaration() {
	bool fl_gl = false;
    Array();
	gl();
	if (c.type_ != "identifier") {
        throw std::string("variable decloration was expected in line " + std::to_string(c.num_of_string_));
	}
	gl();
	if (c.content_ == "=") {
		fl_gl = true;
		gl();
		if (c.type_ == "string literal") {
			gl();
		}
		else {
			Exp();
		}
	}
	while (c.content_ == ",") {
		fl_gl = true;
		gl();
		if (c.type_ != "identifier") {
            throw std::string("variable decloration was expected in line " + std::to_string(c.num_of_string_));
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
		if (fl_gl == true) {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
		}
        throw std::string("expected to get a expression in line " + std::to_string(c.num_of_string_));
	}
	gl();
}
void Operator() {
	if (c.content_ == "cin") {
		gl();
		Cin();
		if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
		}
		gl();
	}
	else if (c.content_ == "cout"){
		gl();
		Cout();
		if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
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
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
		}
		gl();
	}
	else {
		Exp();
		if (c.content_ != ";") {
            throw std::string("expected to get a symbol ; in line " + std::to_string(c.num_of_string_));
		}
		gl();
	}
}
void Block() {
	if (c.content_ != "{") {
        throw std::string("expected to get a symbol { in line " + std::to_string(c.num_of_string_));
	}
	gl();
    
	while (c.content_ != "}") {
        if (c.type_ == "end") {
            throw std::string("expected to get a symbol } in line " + std::to_string(c.num_of_string_));
        }
		if (Type_()) {
			Declaration();
		}
		else if (c.type_ == "identifier") {
			gl();
			if (c.content_ == "(" || c.content_ == "++" || c.content_ == "--") {
				if (c.content_ == "(") {
					gl();
					Enumeration();
					if (c.content_ != ")") {
						"expected to get a symbols )";
					}
				}
				gl();
				if (c.content_ != ";") {
                    throw std::string("expected to get a symbols ; in line " + std::to_string(c.num_of_string_));
				}
				gl();
			}
			else {
				i -= 2;
				gl();
				Exp9();
				if (c.content_ != ";") {
                    throw std::string("expected to get a symbols ; in line " + std::to_string(c.num_of_string_));
				}
				gl();
			}
		}
		else {
			Operator();
		}
	}
	if (c.content_ != "}") {
        throw std::string("expected to get a symbols } in line " + std::to_string(c.num_of_string_));
	}
	gl();
}
void Function() {
    if (i >= lexemes.size() - 1) {
        throw std::string("the main declaration is missing in line " + std::to_string(c.num_of_string_));
    }
	FunctionType();
	bool fl_main = 0;
	if (c.content_ == "main") {
		fl_main = 1;
	}
	else {
		if (c.type_ != "identifier") {
            throw std::string("missing function name in line " + std::to_string(c.num_of_string_));
		}
	}
	gl();
	if (c.content_ != "(") {
        throw std::string("expected to get a symbol ( in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Parameters();
	if (c.content_ != ")") {
        throw std::string("expected to get a symbol ) in line " + std::to_string(c.num_of_string_));
	}
	gl();
	Block();
	if (!fl_main) {
		Global();
	}  
}
void Global() {
	gl();
	gl();
	if (c.content_ == "(") {
		i -= 3;
		gl();
		Function();
	}
	else {
		i -= 3;
		gl();
		Declaration();
		Global();
	}
}
void Program() {
	gl();
	Global();
	if (c.type_ != "end") {
        throw std::string("extra characters at the end of the program");
	}
}

void SyntaxAnalyzer(std::vector<Lexeme> lexemes_) {
	swap(lexemes, lexemes_);
	try {
		Program();
		if (i < lexemes.size()) {
            throw std::string("characters are expected to enter");
		}
	}
	catch (std::string str) {
		std::cout << str;
	}
}
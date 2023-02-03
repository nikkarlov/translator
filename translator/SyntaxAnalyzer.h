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
			throw "expected to get a symbol ]";
		}
		return true;
	}
	if (c.content_ == "(") {
		gl();
		Exp();
		if (c.content_ != ")") {
			throw "expected to get a symbol )";
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
			throw "expected to get a symbol <";
		}
		gl();
		Type();
		if (c.content_ != ">") {
			throw "expected to get a symbol >";
		}
		gl();
		if (c.content_ != "[") {
			throw "expected to get a symbol [";
		}
		gl();
		Exp();
		if (c.content_ != "]") {
			throw "expected to get a symbol ]";
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
		throw "invalid data type";
	}
	gl();
}
void Parameters() {
	if (c.content_ != ")") {
		Type();
		if (c.type_ != "identifier") {
			throw "r";
		}
		gl();
	}
	while (c.content_ == ",") {
		gl();
		Type();
		if (c.type_ != "identifier") {
			throw "variable declaration was expected";
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
				throw "expected to get a symbol )";
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
		throw "expected to get a symbol (";
	}
	else {
		gl();
		Exp8();
		if (c.content_ != ")") {
			throw "expected to get a symbol )";
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
		throw "variable declaration was expected";
	}
	gl();
	if (c.content_ == "[") {
		gl();
		Exp();
		if (c.content_ != "]") {
			throw "expected to get a symbol ]";
		}
		gl();
	}
}
void Exp9() {
	Lvalue();
	if (!Prior9()) {
		throw "expected assignment";
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
		throw "expected to get a symbol (";
	}
	gl();
	Exp();
	if (c.content_ != ")") {
		throw "expected to get a symbol ] )";
	}
	gl();
	Block();
}
void For() {
	if (c.content_ != "(") {
		throw "expected to get a symbol (";
	}
	gl();
	if (Type_()) {
		gl();
		if (c.type_ != "identifier") {
			throw "variable declaration was expected";
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
				throw "variable declaration was expected";
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
			throw "expected to get a symbol ;";
		}
	}
	else if (c.type_ == "identifier") {
		if (c.type_ != "identifier") {
			throw "variable declaration was expected";
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
				throw "variable declaration was expected";
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
			throw "expected to get a symbol ;";
		}
	}
	else if (c.content_ == ";") {
		gl();
	}
	else {
		throw "incorrect cycle definition";
	}
	Exp();
	if (c.content_ != ";") {
		throw " expected to get a symbol ;";
	}
	gl();
	Exp();
	if (c.content_ != ")") {
		throw "expected to get a symbol )";
	}
	gl();
	Block();
}
void If() {
	if (c.content_ != "(") {
		throw "expected to get a symbol (";
	}
	gl();
	Exp();
	if (c.content_ != ")") {
		throw "expected to get a symbol )";
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
	else {
		Exp();
	}
}
void Cout() {
	if (c.content_ != "<<") {
		throw "expected to get a symbols <<";
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
		throw "expected to get a symbols >>";
	}
	gl();
	if (c.type_ != "identifier") {
		throw "variable declaration was expected";
	}
	gl();
	while (c.content_ == ">>") {
		gl();
		if (c.type_ != "identifier") {
			throw "variable declaration was expected";;
		}
		gl();
	}
}
void Declaration() {
	bool fl_gl = false;
	gl();
	if (c.type_ != "identifier") {
		throw "variable decloration was expected";
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
			throw "variable decloration was expected";
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
			throw "expected to get a symbol ;";
		}
		throw "expected to get a symbol }";
	}
	gl();
}
void Operator() {
	if (c.content_ == "cin") {
		gl();
		Cin();
		if (c.content_ != ";") {
			throw "expected to get a symbol ;";
		}
		gl();
	}
	else if (c.content_ == "cout"){
		gl();
		Cout();
		if (c.content_ != ";") {
			throw "expected to get a symbol ;";
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
			throw "expected to get a symbol ;";;
		}
		gl();
	}
	else {
		Exp();
		if (c.content_ != ";") {
			throw "expected to get a symbol ;";;
		}
		gl();
	}
}
void Block() {
	if (c.content_ != "{") {
		throw "expected to get a symbol {";;
	}
	gl();
    
	while (c.content_ != "}") {
        if (c.type_ == "end") {
            throw "expected to get a symbol }";
        }
		if (Type_()) {
			Declaration();
		}
		else if (c.type_ == "identifier") {
			Exp9();
			if (c.content_ != ";") {
				throw "expected to get a symbols ;";
			}
			gl();
		}
		else {
			Operator();
		}
	}
	if (c.content_ != "}") {
		throw "expected to get a symbols }";
	}
	gl();
}
void Function() {
    if (i >= lexemes.size() - 1) {
        throw "the main declaration is missing";
    }
	Type();
	bool fl_main = 0;
	if (c.content_ == "main") {
		fl_main = 1;
	}
	else {
		if (c.type_ != "identifier") {
			throw "missing function name";
		}
	}
	gl();
	if (c.content_ != "(") {
		throw "expected to get a symbol (";
	}
	gl();
	Parameters();
	if (c.content_ != ")") {
		throw "expected to get a symbol )";
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
		throw "extra characters at the end of the program";
	}
}

void SyntaxAnalyzer(std::vector<Lexeme> lexemes_) {
	swap(lexemes, lexemes_);
	try {
		Program();
		if (i < lexemes.size()) {
			throw "characters are expected to enter";
		}
	}
	catch (const char str[]) {
		std::cout << str;
	}
}
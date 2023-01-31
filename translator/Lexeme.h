#pragma once
#include <string>
class Lexeme {
 public:
	Lexeme(){}
	Lexeme(std::string a, std::string b) :
		content_(a),
		type_(b){}
	std::string content_, type_;
};
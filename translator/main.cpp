#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
int main() {
	std::vector<Lexeme> lex = Analiz();
	SyntaxAnalyzer(lex);
	return 0;
}
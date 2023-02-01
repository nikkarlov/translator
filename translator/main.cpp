#include "LexicalAnalyzer.h"
#include "SyntaxAnalyzer.h"
int main() {
	bool fl = 1;
	std::vector<Lexeme> lex = Analiz(fl);
	if (fl) {
		SyntaxAnalyzer(lex);
	}
	return 0;
}
#include "lexical-analyzer.h"
#include "syntax-analyzer.h"
int main() {
    char c = 'd';
    float f = 3.231;
    c = f;
    std::cout << c;
    LexicalAnalyzer lexical_analyzer;
	SyntaxAnalyzer(lexical_analyzer.LexicalAnalysis());
	return 0;
}
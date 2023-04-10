#include <iostream>
#include <algorithm>
#include "Lexer.h"
#include "Parser.h"


int main(void)
{
	std::string strSource = "void main()\
	{\
		int nNum = 5;\
		int nNum;\
		printf(\"Hello World\");\
		printf(\"%d\", 1 + 2 * 3);\
		return;\
	}";

	std::vector<CLexer::stToken> vResult = CLexer::Scan(strSource);
	std::for_each(vResult.begin(), vResult.end(), [](CLexer::stToken& token) {
		printf("%-20s %-5s\n", token.strString.c_str(), CLexer::FindLexToString(token.eLex).c_str());
	});
	//stProgram* pProg = CParser::Parser(vResult);
	//while ()

	//DeletePtr<stProgram>(pProg);

	return 0;
}
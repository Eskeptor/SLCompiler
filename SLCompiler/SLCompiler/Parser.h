#pragma once
#include "Structures.h"
#include "Lexer.h"

typedef std::vector<CLexer::stToken> vstToken;

static class CParser
{
// Enums and Classes, Structures ==========================================================
public:
	enum class eLogType 
	{
		Info,
		Debug,
		Error,
		LogTypeMax
	};
// ========================================================================================


// Variables ==============================================================================
private:
	const static std::string LOG_TYPE[static_cast<int>(eLogType::LogTypeMax)];
// ========================================================================================


// Functions ==============================================================================
public:
	static stProgram* Parser(vstToken vTokens);
	
private:
	inline static void PrintLog(eLogType eType, std::string strLog);

	static stFunction* ParseFunction(vstToken::iterator& iter);
	static stVariable* ParseVariable(vstToken::iterator& iter);
	static stExpStatement* ParseExpStatement(vstToken::iterator& iter);
	static stExpression* ParseExpression(vstToken::iterator& iter);
	static stExpression* ParseAssignmentExpression(vstToken::iterator& iter);
	static stStatement* ParseReturn(vstToken::iterator& iter);
	static stStatement* ParseFor(vstToken::iterator& iter);
	static stStatement* ParseWhile(vstToken::iterator& iter);
	static stStatement* ParseIf(vstToken::iterator& iter);
	static stStatement* ParseSwitch(vstToken::iterator& iter);
	static stStatement* ParseBreak(vstToken::iterator& iter);
	static stStatement* ParseContinue(vstToken::iterator& iter);
	static stStatement* ParsePrint(vstToken::iterator& iter);
	static std::vector<stStatement*> ParseBlock(vstToken::iterator& iter);

	static bool NextIter(CLexer::eLexEnum eLexCheckType, vstToken::iterator& iter, bool bCritical = true);

// ========================================================================================

};
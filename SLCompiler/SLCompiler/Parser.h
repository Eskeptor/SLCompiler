#pragma once
#include <unordered_set>
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
	static std::unordered_set<CLexer::eLexEnum> m_setRelOp;
	static std::unordered_set<CLexer::eLexEnum> m_setCalOp;
// ========================================================================================


// Functions ==============================================================================
public:
	static stProgram* Parser(vstToken vTokens);
	
private:
	inline static void PrintLog(eLogType eType, std::string strLog);

	static stFunction* ParseFunction(CLexer::eLexEnum eType, vstToken::iterator& iter);
	static stVariable* ParseVariable(vstToken::iterator& iter);
	static stExpStatement* ParseExpStatement(vstToken::iterator& iter);
	static stExpression* ParseExpression(vstToken::iterator& iter);
	static stStatement* ParseReturn(CLexer::eLexEnum eType, vstToken::iterator& iter);
	static stStatement* ParseFor(vstToken::iterator& iter);
	static stStatement* ParseWhile(vstToken::iterator& iter);
	static stStatement* ParseIf(vstToken::iterator& iter);
	static stStatement* ParseSwitch(vstToken::iterator& iter);
	static stStatement* ParseBreak(vstToken::iterator& iter);
	static stStatement* ParseContinue(vstToken::iterator& iter);
	static stStatement* ParsePrintf(vstToken::iterator& iter);
	static stExpression* ParseDataType(vstToken::iterator& iter);
	static stExpression* ParseNullData(vstToken::iterator& iter);
	static stExpression* ParseBooleanData(vstToken::iterator& iter);
	static stExpression* ParseIntData(vstToken::iterator& iter);
	static stExpression* ParseDoubleData(vstToken::iterator& iter);
	static stExpression* ParseStringData(vstToken::iterator& iter);
	static stExpression* ParseVoidData(vstToken::iterator& iter);
	static stExpression* ParseArrayData(vstToken::iterator& iter);
	static stExpression* ParseAnd(vstToken::iterator& iter);
	static stExpression* ParseOr(vstToken::iterator& iter);
	static stExpression* ParseRelational(vstToken::iterator& iter);
	static stExpression* ParseArithmetic(vstToken::iterator& iter);
	static stExpression* ParseUnary(vstToken::iterator& iter);
	static stExpression* ParseIdentifier(vstToken::iterator& iter);

	static std::vector<stStatement*> ParseBlock(CLexer::eLexEnum eType, vstToken::iterator& iter);

	static bool NextIter(CLexer::eLexEnum eLexCheckType, vstToken::iterator& iter, bool bCritical = true);

// ========================================================================================

};
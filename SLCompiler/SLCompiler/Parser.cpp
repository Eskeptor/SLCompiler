#include "Parser.h"

const std::string CParser::LOG_TYPE[static_cast<int>(eLogType::LogTypeMax)] =
{
	"Info",
	"Debug",
	"Error"
};

/**
@brief
@param
@return
*/
stProgram* CParser::Parser(vstToken vTokens)
{
	stProgram* prog = new stProgram();
	vstToken::iterator iter = vTokens.begin();

	while (iter->eLex != CLexer::eLexEnum::EndOfLine)
	{
		switch (iter->eLex)
		{
			case CLexer::eLexEnum::Variable:
				break;
			case CLexer::eLexEnum::Function:
				break;
			default:
				break;
		}
	}

	return prog;
}

inline void CParser::PrintLog(eLogType eType, std::string strLog)
{
	printf("[%-5s] %s\n", LOG_TYPE[static_cast<int>(eType)].c_str(), strLog.c_str());
}

/**
@brief
@param
@return
*/
stFunction* CParser::ParseFunction(vstToken::iterator& iter)
{
	stFunction* func = new stFunction();

	// Check function
	NextIter(CLexer::eLexEnum::Function, iter);

	// Check function name (function name is 'Identifier' type)
	func->strName = iter->strString;
	NextIter(CLexer::eLexEnum::Identifier, iter);

	// Check function parameters
	NextIter(CLexer::eLexEnum::LeftParent, iter);
	{
		if (iter->eLex != CLexer::eLexEnum::RightParent)
		{
			while (NextIter(CLexer::eLexEnum::Comma, iter, false))
			{
				func->vParams.push_back(iter->strString);
				NextIter(CLexer::eLexEnum::Identifier, iter);
			}
		}
	}
	NextIter(CLexer::eLexEnum::RightParent, iter);

	// Check function context
	NextIter(CLexer::eLexEnum::LeftBrace, iter);
	{
		func->vBlock = ParseBlock(iter);
	}
	NextIter(CLexer::eLexEnum::RightBrace, iter);

	return func;
}

/**
@brief
@param
@return
*/
stVariable* CParser::ParseVariable(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stExpStatement* CParser::ParseExpStatement(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stExpression* CParser::ParseExpression(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stExpression* CParser::ParseAssignmentExpression(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParseReturn(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParseFor(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParseWhile(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParseIf(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParseSwitch(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParseBreak(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParseContinue(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
stStatement* CParser::ParsePrint(vstToken::iterator& iter)
{
	// TODO Input Code
}

/**
@brief
@param
@return
*/
std::vector<stStatement*> CParser::ParseBlock(vstToken::iterator& iter)
{
	std::vector<stStatement*> vBlock;

	while (iter->eLex != CLexer::eLexEnum::RightBrace)
	{
		switch (iter->eLex)
		{
			case CLexer::eLexEnum::Variable:
				vBlock.push_back(ParseVariable(iter));
				break;
			case CLexer::eLexEnum::Return:
				vBlock.push_back(ParseReturn(iter));
				break;
			case CLexer::eLexEnum::For:
				vBlock.push_back(ParseFor(iter));
				break;
			case CLexer::eLexEnum::While:
				vBlock.push_back(ParseWhile(iter));
				break;
			case CLexer::eLexEnum::If:
				vBlock.push_back(ParseIf(iter));
				break;
			case CLexer::eLexEnum::Switch:
				vBlock.push_back(ParseSwitch(iter));
				break;
			case CLexer::eLexEnum::Break:
				vBlock.push_back(ParseBreak(iter));
				break;
			case CLexer::eLexEnum::Continue:
				vBlock.push_back(ParseContinue(iter));
				break;
			case CLexer::eLexEnum::Print:
				vBlock.push_back(ParsePrint(iter));
				break;
			case CLexer::eLexEnum::EndOfLine:
				PrintLog(eLogType::Error, "EndOfLine checked before RightBrace came.");
				exit(1);
				break;
			default:
				vBlock.push_back(ParseExpStatement(iter));
				break;
		}
	}

	return vBlock;
}

/**
@brief
@param
@return
*/
bool CParser::NextIter(CLexer::eLexEnum eLexCheckType, vstToken::iterator& iter, bool bCritical)
{
	if (iter->eLex != eLexCheckType)
	{
		if (bCritical)
		{
			PrintLog(eLogType::Error, "Next token check failed.");
			exit(1);
		}
		return false;
	}

	++iter;
	return true;
}
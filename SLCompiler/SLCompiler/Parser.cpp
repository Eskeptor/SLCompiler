#include "Parser.h"

// Log type
const std::string CParser::LOG_TYPE[static_cast<int>(eLogType::LogTypeMax)] =
{
	"Info",
	"Debug",
	"Error"
};

// Relational operator set
std::unordered_set<CLexer::eLexEnum> CParser::m_setRelOp = 
{
	CLexer::eLexEnum::RelOpEqual,
	CLexer::eLexEnum::RelOpNotEqual,
	CLexer::eLexEnum::RelOpLessThan,
	CLexer::eLexEnum::RelOpGreaterThan,
	CLexer::eLexEnum::RelOpLessOrEqual,
	CLexer::eLexEnum::RelOpGreaterOrEqual,
};

// Calculational operator set
std::unordered_set<CLexer::eLexEnum> CParser::m_setCalOp =
{
	CLexer::eLexEnum::OpAdd,
	CLexer::eLexEnum::OpSubtract,
	CLexer::eLexEnum::OpMultiply,
	CLexer::eLexEnum::OpDivide,
	CLexer::eLexEnum::OpModulo,
};

/**
@brief		Parser
@param		vTokens			Token
@return		Token to program structure
*/
stProgram* CParser::Parser(vstToken vTokens)
{
	stProgram* pProg = new stProgram();
	vstToken::iterator iter = vTokens.begin();

	while (iter->eLex != CLexer::eLexEnum::EndOfLine)
	{
		switch (iter->eLex)
		{
			case CLexer::eLexEnum::Function:
				pProg->vFunc.push_back(ParseFunction(iter));
				break;
			default:
				break;
		}
	}

	return pProg;
}

inline void CParser::PrintLog(eLogType eType, std::string strLog)
{
	printf("[%-5s] %s\n", LOG_TYPE[static_cast<int>(eType)].c_str(), strLog.c_str());
}

/**
@brief		Function parser
@param		iter		Token iterator
@return		Token to function structure
*/
stFunction* CParser::ParseFunction(vstToken::iterator& iter)
{
	stFunction* pFunc = new stFunction();

	// Check function
	NextIter(CLexer::eLexEnum::Function, iter);

	// Check function name (function name is 'Identifier' type)
	pFunc->strName = iter->strString;
	NextIter(CLexer::eLexEnum::Identifier, iter);

	// Check function parameters
	NextIter(CLexer::eLexEnum::LeftParent, iter);
	{
		if (iter->eLex != CLexer::eLexEnum::RightParent)
		{
			while (NextIter(CLexer::eLexEnum::Comma, iter, false))
			{
				pFunc->vParams.push_back(iter->strString);
				NextIter(CLexer::eLexEnum::Identifier, iter);
			}
		}
	}
	NextIter(CLexer::eLexEnum::RightParent, iter);

	// Check function context
	NextIter(CLexer::eLexEnum::LeftBrace, iter);
	{
		pFunc->vBlock = ParseBlock(iter);
	}
	NextIter(CLexer::eLexEnum::RightBrace, iter);

	return pFunc;
}

/**
@brief		Variable parser
@param		iter		Token iterator
@return		Token to variable structure
*/
stVariable* CParser::ParseVariable(vstToken::iterator& iter)
{
	stVariable* pVar = new stVariable();
	CLexer::eLexEnum eArrType[] = { CLexer::eLexEnum::Void, CLexer::eLexEnum::Int, CLexer::eLexEnum::Double, CLexer::eLexEnum::String };
	int nSize = sizeof(eArrType) / sizeof(CLexer::eLexEnum);

	for (int i = 0; i < nSize; ++i)
	{
		if (NextIter(eArrType[i], iter, false))
		{
			pVar->eType = eArrType[i];
			pVar->strName = iter->strString;
			NextIter(CLexer::eLexEnum::Identifier, iter);
			break;
		}
	}

	NextIter(CLexer::eLexEnum::Assignment, iter);

	pVar->stExp = ParseExpression(iter);
	NextIter(CLexer::eLexEnum::Semicolon, iter);

	return pVar;
}

/**
@brief		Expression statement parser
@param		iter		Token iterator
@return		Token to expression statement structure
*/
stExpStatement* CParser::ParseExpStatement(vstToken::iterator& iter)
{
	stExpStatement* pExp = new stExpStatement();
	pExp->stExp = ParseExpression(iter);
	NextIter(CLexer::eLexEnum::Semicolon, iter);

	return pExp;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stExpression* CParser::ParseExpression(vstToken::iterator& iter)
{
	stExpression* pExp = new stExpression();
	
	// TODO Input Code

	return pExp;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stExpression* CParser::ParseAssignmentExpression(vstToken::iterator& iter)
{
	stExpression* pExp = ParseOr(iter);

	// TODO Input Code

	return pExp;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParseReturn(vstToken::iterator& iter)
{
	stReturn* pReturn = new stReturn();

	// TODO Input Code

	return pReturn;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParseFor(vstToken::iterator& iter)
{
	stFor* pFor = new stFor();

	// TODO Input Code

	return pFor;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParseWhile(vstToken::iterator& iter)
{
	stWhile* pWhile = new stWhile();

	// TODO Input Code

	return pWhile;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParseIf(vstToken::iterator& iter)
{
	stIf* pIf = new stIf();

	// TODO Input Code

	return pIf;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParseSwitch(vstToken::iterator& iter)
{
	stSwitch* pSwitch = new stSwitch();

	// TODO Input Code

	return pSwitch;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParseBreak(vstToken::iterator& iter)
{
	stBreak* pBreak = new stBreak();

	// TODO Input Code

	return pBreak;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParseContinue(vstToken::iterator& iter)
{
	stContinue* pContinue = new stContinue();

	// TODO Input Code

	return pContinue;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stStatement* CParser::ParsePrint(vstToken::iterator& iter)
{
	stPrint* pPrint = new stPrint();

	// TODO Input Code

	return pPrint;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stExpression* CParser::ParseOperand(vstToken::iterator& iter)
{
	stExpression* pExp = nullptr;

	switch (iter->eLex)
	{
		case CLexer::eLexEnum::Null:
			pExp = ParseNullData(iter);
			break;
		case CLexer::eLexEnum::True:
		case CLexer::eLexEnum::False:
			pExp = ParseBooleanData(iter);
			break;
		case CLexer::eLexEnum::Int:
			pExp = ParseIntData(iter);
			break;
		case CLexer::eLexEnum::Double:
			pExp = ParseDoubleData(iter);
			break;
		case CLexer::eLexEnum::String:
			pExp = ParseStringData(iter);
			break;
		case CLexer::eLexEnum::Void:
			pExp = ParseVoidData(iter);
			break;
		case CLexer::eLexEnum::Identifier:
			pExp = ParseIdentifier(iter);
			break;
		case CLexer::eLexEnum::LeftBraket:	// <-- Array
			pExp = ParseArrayData(iter);
			break;
	}

	return pExp;
}

/**
@brief		Null data parser
@param		iter		Token iterator
@return		Token to null data structure
*/
stExpression* CParser::ParseNullData(vstToken::iterator& iter)
{
	stNullData* pNull = new stNullData();
	NextIter(CLexer::eLexEnum::Null, iter);

	return pNull;
}

/**
@brief		Boolean data parser
@param		iter		Token iterator
@return		Token to boolean data structure
*/
stExpression* CParser::ParseBooleanData(vstToken::iterator& iter)
{
	stBoolData* pBool = new stBoolData();
	pBool->bData = iter->eLex == CLexer::eLexEnum::True;
	NextIter(iter->eLex, iter);

	return pBool;
}

/**
@brief		Integer data parser
@param		iter		Token iterator
@return		Token to integer data structure
*/
stExpression* CParser::ParseIntData(vstToken::iterator& iter)
{
	stIntData* pInt = new stIntData();
	pInt->nData = std::stoi(iter->strString);
	NextIter(CLexer::eLexEnum::Int, iter);

	return pInt;
}

/**
@brief		Double data parser
@param		iter		Token iterator
@return		Token to double data structure
*/
stExpression* CParser::ParseDoubleData(vstToken::iterator& iter)
{
	stDoubleData* pDouble = new stDoubleData();
	pDouble->dData = std::stod(iter->strString);
	NextIter(CLexer::eLexEnum::Double, iter);

	return pDouble;
}

/**
@brief		String data parser
@param		iter		Token iterator
@return		Token to string data structure
*/
stExpression* CParser::ParseStringData(vstToken::iterator& iter)
{
	stStringData* pString = new stStringData();
	pString->strData = iter->strString;
	NextIter(CLexer::eLexEnum::String, iter);

	return pString;
}

/**
@brief		Void data parser
@param		iter		Token iterator
@return		Token to void data structure
*/
stExpression* CParser::ParseVoidData(vstToken::iterator& iter)
{
	stVoidData* pVoid = new stVoidData();
	NextIter(CLexer::eLexEnum::Void, iter);

	return pVoid;
}

/**
@brief		Array data parser
@param		iter		Token iterator
@return		Token to array data structure
*/
stExpression* CParser::ParseArrayData(vstToken::iterator& iter)
{
	NextIter(CLexer::eLexEnum::LeftBraket, iter);

	if (iter->eLex != CLexer::eLexEnum::Int)
		return nullptr;

	int nSize = std::stoi(iter->strString);
	if (nSize <= 0)
		return nullptr;

	stArray* pArray = new stArray(nSize);

	NextIter(CLexer::eLexEnum::Int, iter);
	if (iter->eLex != CLexer::eLexEnum::Semicolon)
	{
		// TODO 배열 데이터 넣는 것 만들기
	}
	
	return pArray;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stExpression* CParser::ParseAnd(vstToken::iterator& iter)
{
	stAnd* pAnd = new stAnd();

	//while ()

	return pAnd;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stExpression* CParser::ParseOr(vstToken::iterator& iter)
{
	stOr* pOr = new stOr();

	return pOr;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stExpression* CParser::ParseRelational(vstToken::iterator& iter)
{
	stRelational* pRel = new stRelational();

	// Priority 0: *, /, %
	// Priority 1: +, -

	// TODO Input Code

	return pRel;
}

/**
@brief
@param		iter		Token iterator
@return
*/
stExpression* CParser::ParseArithmetic(vstToken::iterator& iter)
{
	stArithmetic* pArith = new stArithmetic();

	// TODO Input Code

	return pArith;
}

/**
@brief		Unary parser
@param		iter		Token iterator
@return		Token to unary structure
*/
stExpression* CParser::ParseUnary(vstToken::iterator& iter)
{
	stUnary* pUn = new stUnary();

	// TODO Input Code

	return pUn;
}

/**
@brief		Identifier parser
@param		iter		Token iterator
@return		Token to itentifier(stGetVariable) structure
*/
stExpression* CParser::ParseIdentifier(vstToken::iterator& iter)
{
	stGetVariable* pGetVar = new stGetVariable();
	pGetVar->strName = iter->strString;
	NextIter(CLexer::eLexEnum::Identifier, iter);

	return pGetVar;
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
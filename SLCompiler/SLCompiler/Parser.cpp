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
std::unordered_set<CLexer::eLexEnum> CParser::m_setCalOp1 =
{
	CLexer::eLexEnum::OpMultiply,
	CLexer::eLexEnum::OpDivide,
	CLexer::eLexEnum::OpModulo,
};
std::unordered_set<CLexer::eLexEnum> CParser::m_setCalOp2 =
{
	CLexer::eLexEnum::OpAdd,
	CLexer::eLexEnum::OpSubtract,
};

/**
@brief		Parser
@param		vTokens			Token
@return		Token to "Program" structure
*/
stProgram* CParser::Parser(vstToken vTokens)
{
	stProgram* pProg = new stProgram();
	vstToken::iterator iter = vTokens.begin();
	CLexer::eLexEnum eType = CLexer::eLexEnum::Unknown;

	while (iter->eLex != CLexer::eLexEnum::EndOfLine)
	{
		switch (iter->eLex)
		{
			case CLexer::eLexEnum::True:
			case CLexer::eLexEnum::False:
			case CLexer::eLexEnum::Int:
			case CLexer::eLexEnum::Double:
			case CLexer::eLexEnum::String:
			case CLexer::eLexEnum::Void:
				eType = iter->eLex;
				break;
			case CLexer::eLexEnum::Function:
				if (eType == CLexer::eLexEnum::Unknown)
				{
					// Error
					break;
				}
				pProg->vFunc.push_back(ParseFunction(eType, iter));
				eType = CLexer::eLexEnum::Unknown;
				break;
			default:
				break;
		}

		if (pProg->vFunc[pProg->vFunc.size() - 1] == nullptr)
		{
			DeletePtr<stProgram>(pProg);
			return nullptr;
		}
	}

	return pProg;
}

/**
@brief		Print log
@param		eType		Log type
@param		strLog		Log
@return
*/
inline void CParser::PrintLog(eLogType eType, std::string strLog)
{
	printf("[%-5s] %s\n", LOG_TYPE[static_cast<int>(eType)].c_str(), strLog.c_str());
}

/**
@brief		Function parser
@param		eType		Function's return type (use only Return)
@param		iter		Token iterator
@return		Token to "Function" structure
*/
stFunction* CParser::ParseFunction(CLexer::eLexEnum eType, vstToken::iterator& iter)
{
	stFunction* pFunc = new stFunction();

	// Check Function
	NextIter(CLexer::eLexEnum::Function, iter);

	// Check function name (function name is 'Identifier' type)
	pFunc->strName = iter->strString;
	NextIter(CLexer::eLexEnum::Identifier, iter);

	// Check function parameters
	// Check "("
	NextIter(CLexer::eLexEnum::LeftParent, iter);
	{
		// if parameter is not empty
		if (iter->eLex != CLexer::eLexEnum::RightParent)
		{
			// Check next parameters
			// Check comma
			while (NextIter(CLexer::eLexEnum::Comma, iter, false))
			{
				// Check parameter name
				pFunc->vParams.push_back(iter->strString);
				NextIter(CLexer::eLexEnum::Identifier, iter);
			}
		}
	}
	// Check ")"
	NextIter(CLexer::eLexEnum::RightParent, iter);

	// Check function block
	// Check "{"
	NextIter(CLexer::eLexEnum::LeftBrace, iter);
	{
		pFunc->vBlock = ParseBlock(eType, iter);

		if (pFunc->vBlock.size() == 1 &&
			pFunc->vBlock[0] == nullptr)
		{
			PrintLog(eLogType::Error, "Block parse failed.");
			DeletePtr<stFunction>(pFunc);
			return nullptr;
		}
	}
	// Check "}"
	NextIter(CLexer::eLexEnum::RightBrace, iter);

	return pFunc;
}

/**
@brief		Variable parser
@param		iter		Token iterator
@return		Token to "Variable" structure
*/
stVariable* CParser::ParseVariable(vstToken::iterator& iter)
{
	stVariable* pVar = new stVariable();
	CLexer::eLexEnum eArrType[] = { CLexer::eLexEnum::Void, CLexer::eLexEnum::Int, CLexer::eLexEnum::Double, CLexer::eLexEnum::String };
	int nSize = sizeof(eArrType) / sizeof(CLexer::eLexEnum);

	for (int i = 0; i < nSize; ++i)
	{
		// Check Variable
		if (NextIter(eArrType[i], iter, false))
		{
			pVar->eType = eArrType[i];

			// Check variable name
			pVar->strName = iter->strString;
			NextIter(CLexer::eLexEnum::Identifier, iter);
			break;
		}
	}

	// Check assignment and expression
	NextIter(CLexer::eLexEnum::Assignment, iter);
	pVar->stExp = ParseExpression(iter);

	// Check semicolon
	NextIter(CLexer::eLexEnum::Semicolon, iter);

	return pVar;
}

/**
@brief		Expression statement parser
@param		iter		Token iterator
@return		Token to "Expression statement" structure
*/
stExpStatement* CParser::ParseExpStatement(vstToken::iterator& iter)
{
	stExpStatement* pExp = new stExpStatement();

	// Check expression
	pExp->stExp = ParseExpression(iter);

	// Check semicolon
	NextIter(CLexer::eLexEnum::Semicolon, iter);

	return pExp;
}

/**
@brief		Expression parser
@param		iter		Token iterator
@return		Token to "Expression" structure
*/
stExpression* CParser::ParseExpression(vstToken::iterator& iter)
{
	stExpression* pExp = ParseOr(iter);
	
	// TODO Input Code ParseExpression

	return pExp;
}

/**
@brief		Return parser
@param		eType		Return type
@param		iter		Token iterator
@return		Token to "Return statement" structure
*/
stStatement* CParser::ParseReturn(CLexer::eLexEnum eType, vstToken::iterator& iter)
{
	stReturn* pReturn = new stReturn();

	// Check Return
	NextIter(CLexer::eLexEnum::Return, iter);

	// Check return type
	// if function's return
	if (eType != CLexer::eLexEnum::Unknown)
	{
		pReturn->stExp = ParseExpression(iter);

		if (pReturn->stExp == nullptr)
		{
			PrintLog(eLogType::Error, "Return expression is null.");
			DeletePtr<stReturn>(pReturn);
			return nullptr;
		}
	}

	// Check semicolon
	NextIter(CLexer::eLexEnum::Semicolon, iter);

	return pReturn;
}

/**
@brief		For parser
@param		iter		Token iterator
@return		Token to "For statement" structure
*/
stStatement* CParser::ParseFor(vstToken::iterator& iter)
{
	stFor* pFor = new stFor();

	// Check For
	NextIter(CLexer::eLexEnum::For, iter);

	// Check for loop expression
	// Check "("
	NextIter(CLexer::eLexEnum::LeftParent, iter);
	{
		// Check semicolon
		// (if init expression is empty)
		if (NextIter(CLexer::eLexEnum::Semicolon, iter, false) == false)
		{
			// Check "int"
			// (For statement factor must be integer type)
			NextIter(CLexer::eLexEnum::Int, iter);
			pFor->stVar = new stVariable();

			// Check variable name
			pFor->stVar->strName = iter->strString;
			NextIter(CLexer::eLexEnum::Identifier, iter);

			// Check variable expression
			NextIter(CLexer::eLexEnum::Assignment, iter);
			pFor->stVar->stExp = ParseExpression(iter);
			if (pFor->stVar->stExp == nullptr)
			{
				PrintLog(eLogType::Error, "For expression is null.");
				DeletePtr<stFor>(pFor);
				return nullptr;
			}

			// Check semicolon
			NextIter(CLexer::eLexEnum::Semicolon, iter);
		}

		// Check semicolon
		// (if condition expression is empty)
		if (NextIter(CLexer::eLexEnum::Semicolon, iter, false) == false)
		{
			// Check condition expression
			pFor->stCondExp = ParseExpression(iter);
		}

		// Check loop expression
		pFor->stLoopExp = ParseExpression(iter);
	}
	// Check ")"
	NextIter(CLexer::eLexEnum::RightParent, iter);

	// Check "{"
	NextIter(CLexer::eLexEnum::LeftBrace, iter);
	{
		// Check for block
		pFor->stBlock = ParseBlock(CLexer::eLexEnum::Unknown, iter);
	}
	// Check "}"
	NextIter(CLexer::eLexEnum::RightBrace, iter);

	return pFor;
}

/**
@brief		While parser
@param		iter		Token iterator
@return		Token to "While statement" structure
*/
stStatement* CParser::ParseWhile(vstToken::iterator& iter)
{
	stWhile* pWhile = new stWhile();
	
	// Check While
	NextIter(CLexer::eLexEnum::While, iter);

	// Check while loop expression
	// Check "("
	NextIter(CLexer::eLexEnum::LeftParent, iter);
	{
		pWhile->stCondExp = ParseExpression(iter);

		if (pWhile->stCondExp == nullptr)
		{
			PrintLog(eLogType::Error, "While expression is null.");
			DeletePtr<stWhile>(pWhile);
			return nullptr;
		}
	}
	// Check ")"
	NextIter(CLexer::eLexEnum::RightParent, iter);

	// Check "{"
	NextIter(CLexer::eLexEnum::LeftBrace, iter);
	{
		// Check for block
		pWhile->stBlock = ParseBlock(CLexer::eLexEnum::Unknown, iter);
	}
	// Check "}"
	NextIter(CLexer::eLexEnum::RightBrace, iter);

	return pWhile;
}

/**
@brief		If parser
@param		iter		Token iterator
@return		Token to "If statement" structure
*/
stStatement* CParser::ParseIf(vstToken::iterator& iter)
{
	stIf* pIf = new stIf();

	// Check If
	NextIter(CLexer::eLexEnum::If, iter);
	{
		// Check if expression
		// Check "("
		NextIter(CLexer::eLexEnum::LeftParent, iter);
		{
			pIf->stCondStm.push_back(ParseExpression(iter));
		}
		// Check ")"
		NextIter(CLexer::eLexEnum::RightParent, iter);

		// Check "{"
		NextIter(CLexer::eLexEnum::LeftBrace, iter);
		{
			// Check for block
			pIf->vIfBlock.push_back(ParseBlock(CLexer::eLexEnum::Unknown, iter));
		}
		// Check "}"
		NextIter(CLexer::eLexEnum::RightBrace, iter);
	}
	// Check "else if"
	while (NextIter(CLexer::eLexEnum::Elif, iter, false))
	{
		// Check if expression
		// Check "("
		NextIter(CLexer::eLexEnum::LeftParent, iter);
		{
			pIf->stCondStm.push_back(ParseExpression(iter));
		}
		// Check ")"
		NextIter(CLexer::eLexEnum::RightParent, iter);

		// Check "{"
		NextIter(CLexer::eLexEnum::LeftBrace, iter);
		{
			// Check for block
			pIf->vIfBlock.push_back(ParseBlock(CLexer::eLexEnum::Unknown, iter));
		}
		// Check "}"
		NextIter(CLexer::eLexEnum::RightBrace, iter);
	}
	// Check "else"
	if (NextIter(CLexer::eLexEnum::If, iter, false))
	{
		// Check "{"
		NextIter(CLexer::eLexEnum::LeftBrace, iter);
		{
			// Check for block
			pIf->vElseBlock = ParseBlock(CLexer::eLexEnum::Unknown, iter);
		}
		// Check "}"
		NextIter(CLexer::eLexEnum::RightBrace, iter);
	}

	return pIf;
}

/**
@brief		Switch parser
@param		iter		Token iterator
@return		Token to "Switch statement" structure
*/
stStatement* CParser::ParseSwitch(vstToken::iterator& iter)
{
	stSwitch* pSwitch = new stSwitch();

	// Check Switch
	NextIter(CLexer::eLexEnum::Switch, iter);
	// Check Switch expression
	// Check "("
	NextIter(CLexer::eLexEnum::LeftParent, iter);
	{
		pSwitch->stExp = ParseExpression(iter);
	}
	// Check ")"
	NextIter(CLexer::eLexEnum::RightParent, iter);

	// Check "{"
	NextIter(CLexer::eLexEnum::LeftBrace, iter);
	{
		// Check Case
		while (NextIter(CLexer::eLexEnum::Case, iter, false))
		{
			// Check codition statement
			pSwitch->stCondStm.push_back(ParseIntData(iter));
			NextIter(CLexer::eLexEnum::Colon, iter);
			pSwitch->vCaseBlock.push_back(ParseBlock(CLexer::eLexEnum::Unknown, iter));
		}
	}
	// Check "}"
	NextIter(CLexer::eLexEnum::RightBrace, iter);

	return pSwitch;
}

/**	
@brief		Break parser
@param		iter		Token iterator
@return		Token to "Break statement" structure
*/
stStatement* CParser::ParseBreak(vstToken::iterator& iter)
{
	stBreak* pBreak = new stBreak();

	// Check Break
	NextIter(CLexer::eLexEnum::Break, iter);

	// Check semicolon
	NextIter(CLexer::eLexEnum::Semicolon, iter);

	return pBreak;
}

/**
@brief		Continue parser
@param		iter		Token iterator
@return		Token to "Continue statement" structure
*/
stStatement* CParser::ParseContinue(vstToken::iterator& iter)
{
	stContinue* pContinue = new stContinue();

	// Check Continue
	NextIter(CLexer::eLexEnum::Continue, iter);

	// Check semicolon
	NextIter(CLexer::eLexEnum::Semicolon, iter);

	return pContinue;
}

/**
@brief		Printf parser
@param		iter		Token iterator
@return		Token to "Printf statement" structure
*/
stStatement* CParser::ParsePrintf(vstToken::iterator& iter)
{
	stPrint* pPrint = new stPrint();

	// Check Printf
	if (NextIter(CLexer::eLexEnum::Printf, iter, false))
	{
		// Check "("
		NextIter(CLexer::eLexEnum::LeftParent, iter);
		{
			// Type1: printf("String");
			// Type2: printf("%d", num);
			while (iter->eLex != CLexer::eLexEnum::RightParent)
			{
				// TODO print문 파싱
			}
		}
		// Check ")"
		NextIter(CLexer::eLexEnum::RightParent, iter);
	}
	else
	{
		PrintLog(eLogType::Error, "Print statement is null.");
		DeletePtr<stPrint>(pPrint);
		return nullptr;
	}

	return pPrint;
}

/**
@brief		Data type parser
@param		iter		Token iterator
@return		Token to "Data type expression" structure
*/
stExpression* CParser::ParseDataType(vstToken::iterator& iter)
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
@return		Token to "Null data" structure
*/
stExpression* CParser::ParseNullData(vstToken::iterator& iter)
{
	stNullData* pNull = new stNullData();

	// Check Null data
	NextIter(CLexer::eLexEnum::Null, iter);

	return pNull;
}

/**
@brief		Boolean data parser
@param		iter		Token iterator
@return		Token to "Boolean data" structure
*/
stExpression* CParser::ParseBooleanData(vstToken::iterator& iter)
{
	stBoolData* pBool = new stBoolData();

	// Check Boolean (True or False) data
	pBool->bData = iter->eLex == CLexer::eLexEnum::True;
	NextIter(iter->eLex, iter);

	return pBool;
}

/**
@brief		Integer data parser
@param		iter		Token iterator
@return		Token to "Integer data" structure
*/
stExpression* CParser::ParseIntData(vstToken::iterator& iter)
{
	stIntData* pInt = new stIntData();

	// Check Integer data
	pInt->nData = std::stoi(iter->strString);
	NextIter(CLexer::eLexEnum::Int, iter);

	return pInt;
}

/**
@brief		Double data parser
@param		iter		Token iterator
@return		Token to "Double data" structure
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
@return		Token to "String data" structure
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
@return		Token to "Void data" structure
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
@return		Token to "Array data" structure
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
@brief		And parser
@param		iter		Token iterator
@return		Token to "And expression" structure
*/
stExpression* CParser::ParseAnd(vstToken::iterator& iter)
{
	stExpression* pRel = ParseRelational(iter);

	while (NextIter(CLexer::eLexEnum::LogicOpAnd, iter, false))
	{
		stAnd* pAnd = new stAnd();
		pAnd->stLeft = pRel;
		pAnd->stRight = ParseRelational(iter);
		pRel = pAnd;
	}

	return pRel;
}

/**
@brief		Or parser
@param		iter		Token iterator
@return		Token to "Or expression" structure
*/
stExpression* CParser::ParseOr(vstToken::iterator& iter)
{
	stExpression* pAnd = ParseAnd(iter);

	while (NextIter(CLexer::eLexEnum::LogicOpOr, iter, false))
	{
		stOr* pOr = new stOr();
		pOr->stLeft = pAnd;
		pOr->stRight = ParseAnd(iter);
		pAnd = pOr;
	}

	return pAnd;
}

/**
@brief		Relational parser
@param		iter		Token iterator
@return		Token to "Relational expression" structure
*/
stExpression* CParser::ParseRelational(vstToken::iterator& iter)
{
	stExpression* pArith = ParseArithmetic(false, iter);

	while (m_setRelOp.count(iter->eLex))
	{
		stRelational* pRel = new stRelational();
		pRel->eType = iter->eLex;
		NextIter(iter->eLex, iter);
		pRel->stLeft = pArith;
		pRel->stRight = ParseArithmetic(false, iter);
		pArith = pRel;
	}

	return pArith;
}

/**
@brief		Arithmetic parser
@param		iter		Token iterator
@return		Token to "Arithmetic expression" structure
*/
stExpression* CParser::ParseArithmetic(bool bIsPriority, vstToken::iterator& iter)
{
	std::unordered_set<CLexer::eLexEnum>* pOp = nullptr;
	stExpression* pExp = nullptr;

	if (bIsPriority)
	{
		pOp = &m_setCalOp1;
		pExp = ParseUnary(iter);
	}
	else 
	{
		pOp = &m_setCalOp2;
		pExp = ParseArithmetic(true, iter);
	}

	while (pOp->count(iter->eLex))
	{
		stArithmetic* pArith = new stArithmetic();
		pArith->eType = iter->eLex;
		NextIter(iter->eLex, iter);
		pArith->stLeft = pExp;
		pArith->stRight = bIsPriority ? ParseUnary(iter) : ParseArithmetic(true, iter);
		pExp = pArith;
	}

	return pExp;
}

/**
@brief		Unary parser
@param		iter		Token iterator
@return		Token to "Unary expression" structure
*/
stExpression* CParser::ParseUnary(vstToken::iterator& iter)
{
	if (m_setCalOp2.count(iter->eLex))
	{
		stUnary* pUn = new stUnary();
		pUn->eType = iter->eLex;
		NextIter(iter->eLex, iter);
		pUn->stSubExp = ParseUnary(iter);
		return pUn;
	}

	return ParseDataType(iter);
}

/**
@brief		Identifier parser
@param		iter		Token iterator
@return		Token to "Identifier expression" structure
*/
stExpression* CParser::ParseIdentifier(vstToken::iterator& iter)
{
	stGetVariable* pGetVar = new stGetVariable();
	pGetVar->strName = iter->strString;
	NextIter(CLexer::eLexEnum::Identifier, iter);

	return pGetVar;
}

/**
@brief		Block parser
@param		eType		Return type (use only Return)
@param		iter		Token iterator
@return		Token to "Block statements" structure
*/
std::vector<stStatement*> CParser::ParseBlock(CLexer::eLexEnum eType, vstToken::iterator& iter)
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
				vBlock.push_back(ParseReturn(eType, iter));
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
			case CLexer::eLexEnum::EndOfLine:
				PrintLog(eLogType::Error, "EndOfLine checked before RightBrace came.");
				vBlock.push_back(nullptr);
				break;
			default:
				vBlock.push_back(ParseExpStatement(iter));
				break;
		}

		if (vBlock[vBlock.size() - 1] == nullptr)
		{
			vBlock.clear();
			vBlock.push_back(nullptr);
			break;
		}
	}

	return vBlock;
}

/**
@brief		Increment token iterator
@param		eLexCheckType		Current iterator's eLex value
@param		iter				Token iterator
@param		bCritical			If eLexCheckType and iter->eLex are not the same, whether to terminate
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
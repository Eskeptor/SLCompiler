#include "Lexer.h"

// Lexer Map initialize
std::unordered_map<std::string, CLexer::eLexEnum> CLexer::m_LexMap =
{
	std::make_pair("unknown", eLexEnum::Unknown),
	std::make_pair("null", eLexEnum::Null),
	std::make_pair("true", eLexEnum::True),
	std::make_pair("false", eLexEnum::False),
	std::make_pair("int", eLexEnum::Int),
	std::make_pair("double", eLexEnum::Double),
	std::make_pair("string", eLexEnum::String),
	std::make_pair("void", eLexEnum::Void),
	std::make_pair("$identifier", eLexEnum::Identifier),
	std::make_pair("return", eLexEnum::Return),
	std::make_pair("for", eLexEnum::For),
	std::make_pair("while", eLexEnum::While),
	std::make_pair("if", eLexEnum::If),
	std::make_pair("elif", eLexEnum::Elif),
	std::make_pair("else", eLexEnum::Else),
	std::make_pair("switch", eLexEnum::Switch),
	std::make_pair("case", eLexEnum::Case),
	std::make_pair("default", eLexEnum::Default),
	std::make_pair("break", eLexEnum::Break),
	std::make_pair("continue", eLexEnum::Continue),
	std::make_pair("printf", eLexEnum::Printf),
	std::make_pair("&&", eLexEnum::LogicOpAnd),
	std::make_pair("||", eLexEnum::LogicOpOr),
	std::make_pair("==", eLexEnum::RelOpEqual),
	std::make_pair("!=", eLexEnum::RelOpNotEqual),
	std::make_pair("<", eLexEnum::RelOpLessThan),
	std::make_pair(">", eLexEnum::RelOpGreaterThan),
	std::make_pair("<=", eLexEnum::RelOpLessOrEqual),
	std::make_pair(">=", eLexEnum::RelOpGreaterOrEqual),
	std::make_pair("+", eLexEnum::OpAdd),
	std::make_pair("-", eLexEnum::OpSubtract),
	std::make_pair("*", eLexEnum::OpMultiply),
	std::make_pair("/", eLexEnum::OpDivide),
	std::make_pair("%", eLexEnum::OpModulo),
	std::make_pair("=", eLexEnum::Assignment),
	std::make_pair(".", eLexEnum::Period),
	std::make_pair(",", eLexEnum::Comma),
	std::make_pair(":", eLexEnum::Colon),
	std::make_pair(";", eLexEnum::Semicolon),
	std::make_pair("(", eLexEnum::LeftParent),
	std::make_pair(")", eLexEnum::RightParent),
	std::make_pair("{", eLexEnum::LeftBrace),
	std::make_pair("}", eLexEnum::RightBrace),
	std::make_pair("[", eLexEnum::LeftBraket),
	std::make_pair("]", eLexEnum::RightBraket),
};

// Lex string
std::string CLexer::m_strArrLex[static_cast<int>(CLexer::eLexEnum::EndOfLine)] =
{
	"Unknown",
	"Null",
	"True",
	"False",
	"Int",
	"Double",
	"String",
	"Void",
	"$Identifier",
	"Return",
	"For",
	"While",
	"If",
	"Elif",
	"Else",
	"Switch",
	"Case",
	"Default",
	"Break",
	"Continue",
	"Printf",
	"LogicOpAnd",
	"LogicOpOr",
	"RelOpEqual",
	"RelOpNotEqual",
	"RelOpLessThan",
	"RelOpGreaterThan",
	"RelOpLessOrEqual",
	"RelOpGreaterOrEqual",
	"OpAdd",
	"OpSubtract",
	"OpMultiply",
	"OpDivide",
	"OpModulo",
	"Assignment",
	"Period",
	"Comma",
	"Colon",
	"Semicolon",
	"LeftParent",
	"RightParent",
	"LeftBrace",
	"RightBrace",
	"LeftBraket",
	"RightBraket",
	"$Variable",
	"$Function",
};

/**
@brief		Lexer Scanner
@param		strSourceCode		Source Code
@return		Source Code to token
*/
std::vector<CLexer::stToken> CLexer::Scan(std::string strSourceCode)
{
	// Add End Of Source Character
	strSourceCode += '\0';

	std::vector<stToken> vResult;
	siter iter = strSourceCode.begin();
	bool bIsEndOfSource = false;
	eCharType eType = eCharType::Unknown;
	int nPrevIdentifierIdx = -1;
	int nCurChIdx = -1;
	
	while (iter != strSourceCode.end() &&
		   bIsEndOfSource == false)
	{
		eType = CheckCharType(*iter);
		++nCurChIdx;

		switch (eType)
		{
			case eCharType::Whitespace:
				++iter;
				break;
			case eCharType::Number:
				vResult.push_back(ScanNumber(iter));
				break;
			case eCharType::String:
				vResult.push_back(ScanString(iter));
				break;
			case eCharType::IdentifierKeyword:
			{
				stToken stTokenData = ScanIdentifierKeyword(iter);
				vResult.push_back(stTokenData);

				if (stTokenData.eLex == eLexEnum::Identifier)
					nPrevIdentifierIdx = (int)vResult.size() - 1;
				break;
			}
			case eCharType::OperatorPuncutator:
			{
				stToken stTokenData = ScanOperPunc(iter);
				vResult.push_back(stTokenData);

				if (nPrevIdentifierIdx != -1)
				{
					vResult.at(nPrevIdentifierIdx).eLex = stTokenData.eLex == eLexEnum::LeftParent ? eLexEnum::Function : eLexEnum::Variable;
					nPrevIdentifierIdx = -1;
				}
				break;
			}
			case eCharType::EndOfSource:
				bIsEndOfSource = true;
				break;
			case eCharType::Unknown:
			default:
				printf("Unknown Type : Error Position %s <<-\n", strSourceCode.substr(nCurChIdx).c_str());
				exit(1);
				break;
		}
	}

	return vResult;
}

/**
@brief		Check Charactor Type
@param		ch		Charactor
@return		Charactor Type (enum eCharType)
*/
CLexer::eCharType CLexer::CheckCharType(char ch)
{
	// Check End Of Source
	if (ch == '\0')
		return eCharType::EndOfSource;

	// Check Whitespace
	if (ch == ' ' ||
		ch == '\t' ||
		ch == '\r' ||
		ch == '\n')
		return eCharType::Whitespace;

	// Check Number (int, double)
	if (ch >= '0' &&
		ch <= '9')
		return eCharType::Number;

	// Check String (string)
	if (ch == '\"')
		return eCharType::String;

	// Check Identifier or Keyword
	if ((ch >= 'a' && ch <= 'z') ||
		(ch >= 'A' && ch <= 'Z'))
		return eCharType::IdentifierKeyword;

	// Check Operator or Punctuator
	if ((ch >= 33/* ! */ && ch <= 47/* / */ && ch != '\"' /* 34 */) ||
		(ch >= 58/* : */ && ch <= 64/* @ */) ||
		(ch >= 91/* [ */ && ch <= 96/* ` */) ||
		(ch >= 123/* { */ && ch <= 126/*~*/))
		return eCharType::OperatorPuncutator;

	return eCharType::Unknown;
}

/**
@brief		Scan Number
@param		iter		Scanning point
@return		If the scan result is a number, return {eLexEnum::Int or Double, Number string}, otherwise return {eLexEnum::Unknown, string}
*/
CLexer::stToken CLexer::ScanNumber(siter& iter)
{
	std::string strString = "";
	eLexEnum eLex = eLexEnum::Unknown;

	while (CheckCharType(*iter) == eCharType::Number)
	{
		eLex = eLexEnum::Int;
		strString += *iter;
		++iter;
	}

	// Decimal Point
	if (*iter == '.')
	{
		strString += *iter;
		++iter;

		while (CheckCharType(*iter) == eCharType::Number);
		{
			eLex = eLexEnum::Double;
			strString += *iter;
			++iter;
		}
	}

	// If the value of A is empty, it is not a numeric type.
	if (strString[strString.size() - 1] == '.')
		eLex = eLexEnum::Unknown;
	
	return stToken(eLex, strString);
}

/**
@brief		Scan String
@param		iter		Scanning point
@return		If the scan result is a string, return {eLexEnum::String, string}, otherwise return {eLexEnum::Unknown, string}
*/
CLexer::stToken CLexer::ScanString(siter& iter)
{
	std::string strString = "";
	eLexEnum eLex = eLexEnum::Unknown;
	eCharType eType = eCharType::Unknown;

	// First character is '
	if (CheckCharType(*iter) == eCharType::String)
	{
		eLex = eLexEnum::String;
		++iter;

		// Except for the first and last characters, the rest of the characters are A.
		while (true)
		{
			eType = CheckCharType(*iter);
			if (eType == eCharType::String)
			{
				++iter;
				break;
			}

			if (eType == eCharType::EndOfSource)
				break;

			strString += *iter;
			++iter;
		}
	}

	return stToken(eLex, strString);
}

/**
@brief		Scan Identifier or Keyword
@param		iter		Scanning point
@return		If the scan result is a identifier or keyword, return {Identifier or Keyword, string}, otherwise return {eLexEnum::Unknown, string}
*/
CLexer::stToken CLexer::ScanIdentifierKeyword(siter& iter)
{
	std::string strString = "";
	eLexEnum eLex = eLexEnum::Unknown;

	while (CheckCharType(*iter) == eCharType::IdentifierKeyword)
	{
		strString += *iter;
		++iter;
	}

	eLex = FindLex(strString);
	if (eLex == eLexEnum::Unknown)
		eLex = eLexEnum::Identifier;

	return stToken(eLex, strString);
}

/**
@brief		Scan Operator or Punctuator
@param		iter		Scanning point
@return		If the scan result is a Operator or Punctuator, return {Operator or Punctuator, string}, otherwise return {eLexEnum::Unknown, string}
*/
CLexer::stToken CLexer::ScanOperPunc(siter& iter)
{
	std::string strString = "";
	eLexEnum eLex = eLexEnum::Unknown;

	while (CheckCharType(*iter) == eCharType::OperatorPuncutator)
	{
		strString += *iter;
		++iter;
	}

	while (strString.empty() == false)
	{
		if (FindLex(strString) != eLexEnum::Unknown)
			break;

		strString.pop_back();
		--iter;
	}

	eLex = strString.empty() ? eLexEnum::Unknown : FindLex(strString);

	return stToken(eLex, strString);
}
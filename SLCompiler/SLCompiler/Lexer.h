#pragma once
#include <string>
#include <unordered_map>
#include <vector>

typedef std::string::iterator siter;

static class CLexer
{
// Enums and Classes, Structures ==========================================================
public:
	// Lex Enums
	enum class eLexEnum
	{
		Unknown,				// Unknown Type

		Null,					// null
		True,					// true (boolean)
		False,					// false (boolean)
		Int,					// int
		Double,					// double
		String,					// string
		Void,					// void
		Identifier,				// identifier (Variable name, Function name)

		Return,					// return
		For,					// for
		While,					// while
		If,						// if
		Elif,					// elif
		Else,					// else
		Switch,					// switch
		Case,					// case
		Default,				// default
		Break,					// break
		Continue,				// continue

		Print,					// print()
		Printf,					// printf()

		LogicOpAnd,				// &&
		LogicOpOr,				// ||

		RelOpEqual,				// ==
		RelOpNotEqual,			// !=
		RelOpLessThan,			// <
		RelOpGreaterThan,		// >
		RelOpLessOrEqual,		// <=
		RelOpGreaterOrEqual,	// >=

		OpAdd,					// +
		OpSubtract,				// -
		OpMultiply,				// *
		OpDivide,				// /
		OpModulo,				// %

		Assignment,				// =

		Period,					// .
		Comma,					// ,
		Colon,					// :
		Semicolon,				// ;
		LeftParent,				// (
		RightParent,			// )
		LeftBrace,				// {
		RightBrace,				// }
		LeftBraket,				// [
		RightBraket,			// ]

		Variable,				// Variable
		Function,				// Function

		EndOfLine,				// #EOL
	};

	enum class eCharType
	{
		Unknown,
		Whitespace,
		Number,
		String,
		IdentifierKeyword,
		OperatorPuncutator,
		EndOfSource,
	};

	struct stToken
	{
	public:
		eLexEnum eLex = eLexEnum::Null;
		std::string strString = "";

		stToken() {}
		stToken(eLexEnum eLex, std::string strString)
			: eLex(eLex), strString(strString) {}
	};
// ========================================================================================


// Variables ==============================================================================
private:
	static std::unordered_map<std::string, eLexEnum> m_LexMap;
	static std::string m_nLexNumbers[static_cast<int>(eLexEnum::EndOfLine)];
// ========================================================================================


// Functions ==============================================================================
public:
	static std::vector<stToken> Scan(std::string strSourceCode);
	inline static std::string FindLexToString(eLexEnum eLex)
	{
		return m_nLexNumbers[static_cast<int>(eLex)];
	}

private:
	inline static eLexEnum FindLex(std::string strLex)
	{
		if (m_LexMap.find(strLex) != m_LexMap.end())
			return m_LexMap.at(strLex);
		return eLexEnum::Unknown;
	}
	static eCharType CheckCharType(char c);
	static stToken ScanNumber(siter& iter);
	static stToken ScanString(siter& iter);
	static stToken ScanIdentifierKeyword(siter& iter);
	static stToken ScanOperPunc(siter& iter);
// ========================================================================================

};
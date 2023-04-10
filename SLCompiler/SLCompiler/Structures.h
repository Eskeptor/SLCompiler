#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include "Lexer.h"

#define CREATE_CHS(X)	char* chs = new char[X + 1]; \
						memset(chs, 32, sizeof(char)* X); \
						chs[X] = '\0';
#define DELETE_CHS		DeleteArray<char>(chs);

/**
@brief		Delete pointer vector
@param		v			Vector
@return
*/
template <typename T>
void DeleteVectorPtrArgs(std::vector<T*>& v)
{
	int nSize = (int)v.size();

	if (nSize == 0)
		return;

	for (int i = 0; i < nSize; ++i)
	{
		delete v[i];
		v[i] = nullptr;
	}
	v.clear();
}

/**
@brief		Delete pointer
@param		p			Pointer
@return
*/
template <typename T>
void DeletePtr(T* p)
{
	delete p;
	p = nullptr;
}

/**
@brief		Delete dynamic 1D array
@param		p			1D Array
@return
*/
template <typename T>
void DeleteArray(T p[])
{
	delete[] p;
	p = nullptr;
}

// Statement structure
struct stStatement
{
public:
	virtual void Print(int nSpace) = 0;
};

// Expression structure
struct stExpression
{
public:
	virtual void Print(int nSpace) = 0;
};

// Statement that owns an Expression structure
struct stExpStatement : stStatement
{
public:
	// Expression
	stExpression* stExp;

	stExpStatement()
		: stExp(nullptr)
	{}

	~stExpStatement()
	{
		DeletePtr<stExpression>(stExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sExp Statement:\n", chs);
		if (stExp != nullptr)
			stExp->Print(nSpace + 1);
		DELETE_CHS;
	}
};

// Function structure
struct stFunction : stStatement
{
public:
	// Function name
	std::string strName;
	// Function parameter name
	std::vector<std::string> vParams;
	// Function block
	std::vector<stStatement*> vBlock;

	stFunction()
		: strName("")
	{}

	~stFunction()
	{
		DeleteVectorPtrArgs<stStatement>(vBlock);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		std::string strParam = "";
		std::for_each(vParams.begin(), vParams.end(), [&strParam](std::string& str) {strParam += strParam + " "; });
		printf("%sFunction %s(%s):\n", chs, strName.c_str(), strParam.c_str());
		std::for_each(vBlock.begin(), vBlock.end(), [&nSpace](stStatement* pState) {pState->Print(nSpace + 1); });
		DELETE_CHS;
	}
};

// Variable structure
struct stVariable : stStatement
{
public:
	// Variable name
	std::string strName;
	// Expression
	stExpression* stExp;
	// Data type
	CLexer::eLexEnum eType;

	stVariable()
		: strName(""), stExp(nullptr), eType(CLexer::eLexEnum::Int)
	{}

	~stVariable()
	{
		DeletePtr<stExpression>(stExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sVariable(%s) %s:\n", chs, CLexer::FindLexToString(eType).c_str(), strName.c_str());
		if (stExp != nullptr)
			stExp->Print(nSpace + 1);
		DELETE_CHS;
	}
};

// Return statement structure
struct stReturn : stStatement
{
public:
	// Expression
	stExpression* stExp;

	stReturn()
		: stExp(nullptr)
	{}

	~stReturn()
	{
		DeletePtr<stExpression>(stExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sReturn:\n", chs);
		if (stExp != nullptr)
			stExp->Print(nSpace + 1);
		DELETE_CHS;
	}
};

// For statement structure
struct stFor : stStatement
{
public:
	// For variable (Init-statement)
	stVariable* stVar;
	// Condition expression
	stExpression* stCondExp;
	// Loop expression
	stExpression* stLoopExp;
	// For block
	std::vector<stStatement*> stBlock;

	stFor()
		: stVar(nullptr), stCondExp(nullptr), stLoopExp(nullptr)
	{}

	~stFor()
	{
		DeletePtr<stVariable>(stVar);
		DeletePtr<stExpression>(stCondExp);
		DeletePtr<stExpression>(stLoopExp);
		DeleteVectorPtrArgs<stStatement>(stBlock);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sFor:\n", chs);
		printf("%s Init-statement:\n", chs);
		if (stVar != nullptr)
			stVar->Print(nSpace + 2);
		printf("%s Condition-expression:\n", chs);
		if (stCondExp != nullptr)
			stCondExp->Print(nSpace + 2);
		printf("%s Loop-expression:\n", chs);
		if (stLoopExp != nullptr)
			stLoopExp->Print(nSpace + 2);
		printf("%s Block:\n", chs);
		std::for_each(stBlock.begin(), stBlock.end(), [&nSpace](stStatement* pState) {pState->Print(nSpace + 2); });
		DELETE_CHS;
	}
};

// While statement structure
struct stWhile : stStatement
{
public:
	// Condition expression
	stExpression* stCondExp;
	// While block
	std::vector<stStatement*> stBlock;

	stWhile()
		: stCondExp(nullptr)
	{}

	~stWhile()
	{
		DeletePtr<stExpression>(stCondExp);
		DeleteVectorPtrArgs<stStatement>(stBlock);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sWhile:\n", chs);
		printf("%s Condition-expression:\n", chs);
		if (stCondExp != nullptr)
			stCondExp->Print(nSpace + 2);
		printf("%s Block:\n", chs);
		std::for_each(stBlock.begin(), stBlock.end(), [&nSpace](stStatement* pState) {pState->Print(nSpace + 2); });
		DELETE_CHS;
	}
};

// If statement structure
struct stIf : stStatement
{
public:
	// If condition statements
	std::vector<stExpression*> stCondStm;
	// If expression block
	std::vector<std::vector<stStatement*>> vIfBlock;
	// Else expression block
	std::vector<stStatement*> vElseBlock;

	stIf()
	{}

	~stIf()
	{
		DeleteVectorPtrArgs<stExpression>(stCondStm);
		DeleteVectorPtrArgs<stStatement>(vElseBlock);

		int nSize = (int)vIfBlock.size();
		if (nSize == 0)
			return;
		for (int i = 0; i < nSize; ++i)
			DeleteVectorPtrArgs<stStatement>(vIfBlock[i]);
	}

	void Print(int nSpace) override
	{
		_ASSERT(stCondStm.size() == vIfBlock.size());

		CREATE_CHS(nSpace);
		int nSize = (int)stCondStm.size();
		for (int i = 0; i < nSize; ++i)
		{
			if (i == 0)
				printf("%sIf:\n", chs);
			else
				printf("%sElif:\n", chs);
			printf("%s Condition-expression:\n", chs);
			stCondStm[i]->Print(nSpace + 2);
			printf("%s Block:\n", chs);
			std::for_each(vIfBlock[i].begin(), vIfBlock[i].end(), [&nSpace](stStatement* pState) {pState->Print(nSpace + 2); });
		}
		printf("%sElse:\n", chs);
		printf("%s Block:\n", chs);
		std::for_each(vElseBlock.begin(), vElseBlock.end(), [&nSpace](stStatement* pState) {pState->Print(nSpace + 2); });

		DELETE_CHS;
	}
};

// Switch statement structure
struct stSwitch : stStatement
{
public:
	// Switch condition statements
	stExpression* stExp;
	// Case condition statement
	std::vector<stExpression*> stCondStm;
	// Switch case, default statements block
	std::vector<std::vector<stStatement*>> vCaseBlock;

	stSwitch()
		: stExp(nullptr)
	{}

	~stSwitch()
	{
		DeletePtr<stExpression>(stExp);
		DeleteVectorPtrArgs<stExpression>(stCondStm);

		int nSize = (int)vCaseBlock.size();
		if (nSize == 0)
			return;
		for (int i = 0; i < nSize; ++i)
			DeleteVectorPtrArgs<stStatement>(vCaseBlock[i]);
	}

	void Print(int nSpace) override
	{
		_ASSERT(stCondStm.size() == vCaseBlock.size());

		CREATE_CHS(nSpace);
		printf("%sSwitch:\n", chs);
		printf("%s Init-statement:\n", chs);
		if (stExp != nullptr)
			stExp->Print(nSpace + 2);
		int nSize = (int)stCondStm.size();
		for (int i = 0; i < nSize; ++i)
		{
			printf("%s Case:\n", chs);
			printf("%s Condition-expression:\n", chs);
			stCondStm[i]->Print(nSpace + 2);
			printf("%s Block:\n", chs);
			std::for_each(vCaseBlock[i].begin(), vCaseBlock[i].end(), [&nSpace](stStatement* pState) {pState->Print(nSpace + 2); });
		}
		DELETE_CHS;
	}
};

// Break statement structure
struct stBreak : stStatement
{
public:
	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sBreak:\n", chs);
		DELETE_CHS;
	}
};

// Continue statement Structure
struct stContinue : stStatement
{
public:
	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sContinue:\n", chs);
		DELETE_CHS;
	}
};

// Print statement structure
struct stPrint : stStatement
{
public:
	// Print format
	std::string strFormat;
	// Print arguments
	std::vector<stExpression*> stArgs;

	stPrint()
		: strFormat("")
	{}

	~stPrint()
	{
		DeleteVectorPtrArgs<stExpression>(stArgs);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sPrint:\n", chs);
		printf("%s Format: %s\n", chs, strFormat.c_str());
		printf("%s Arguments:\n", chs);
		std::for_each(stArgs.begin(), stArgs.end(), [&nSpace](stExpression* pExp) {pExp->Print(nSpace + 2); });
		DELETE_CHS;
	}
};

// Null data type structure
struct stNullData : stExpression
{
public:
	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sNull Data:\n", chs);
		DELETE_CHS;
	}
};

// Boolean data type structure
struct stBoolData : stExpression
{
public:
	// Data
	bool bData;

	stBoolData()
		: bData(false)
	{}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sBoolean Data: %s\n", chs, bData ? "true" : "false");
		DELETE_CHS;
	}
};

// Integer data type structure
struct stIntData : stExpression
{
public:
	// Data
	int nData;

	stIntData()
		: nData(0)
	{}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sInteger Data: %d\n", chs, nData);
		DELETE_CHS;
	}
};

// Double data type structure
struct stDoubleData : stExpression
{
public:
	// Data
	double dData;

	stDoubleData()
		: dData(0.0)
	{}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sDouble Data: %f\n", chs, dData);
		DELETE_CHS;
	}
};

// String data type structure
struct stStringData : stExpression
{
public:
	// Data
	std::string strData;

	stStringData()
		: strData("")
	{}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sString Data: %s\n", chs, strData.c_str());
		DELETE_CHS;
	}
};

// Void(pointer) data type structure
struct stVoidData : stExpression
{
public:
	// Data
	void* pVoid;

	stVoidData()
		: pVoid(nullptr)
	{}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sVoid Data: %p\n", chs, &pVoid);
		DELETE_CHS;
	}
};

// Array type structure
struct stArray : stExpression
{
private:
	// Array
	stExpression** m_stArrExp;
	int m_nSize;

public:
	stArray(int nSize)
		: m_stArrExp(nullptr), m_nSize(0)
	{
		if (nSize <= 0)
			return;

		m_nSize = nSize;
		m_stArrExp = new stExpression* [nSize];
	}

	~stArray()
	{
		Destroy();
	}

	void Destroy()
	{
		m_nSize = 0;

		if (m_stArrExp != nullptr)
		{
			delete[] m_stArrExp;
			m_stArrExp = nullptr;
		}
	}

	stExpression* Get(int nIdx)
	{
		if (nIdx <= 0 ||
			nIdx >= m_nSize)
			return nullptr;

		if (m_stArrExp == nullptr)
			return nullptr;

		return m_stArrExp[nIdx];
	}

	void Set(int nIdx, stExpression* pExp)
	{
		if (nIdx <= 0 ||
			nIdx >= m_nSize)
			return;

		if (m_stArrExp == nullptr)
			return;

		delete m_stArrExp[nIdx];
		m_stArrExp[nIdx] = pExp;
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		// TODO Print Array
		DELETE_CHS;
	}
};

// And expression structure
struct stAnd : stExpression
{
public:
	// Left side
	stExpression* stLeft;
	// Right side
	stExpression* stRight;

	stAnd()
		: stLeft(nullptr), stRight(nullptr)
	{}

	~stAnd()
	{
		DeletePtr<stExpression>(stLeft);
		DeletePtr<stExpression>(stRight);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sAnd:\n", chs);
		printf("%s Left:\n", chs);
		if (stLeft != nullptr)
			stLeft->Print(nSpace + 2);
		printf("%s Right:\n", chs);
		if (stRight != nullptr)
			stRight->Print(nSpace + 2);
		DELETE_CHS;
	}
};

// Or expression structure
struct stOr : stExpression
{
public:
	// Left side
	stExpression* stLeft;
	// Right side
	stExpression* stRight;

	stOr()
		: stLeft(nullptr), stRight(nullptr)
	{}

	~stOr()
	{
		DeletePtr<stExpression>(stLeft);
		DeletePtr<stExpression>(stRight);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sOr:\n", chs);
		printf("%s Left:\n", chs);
		if (stLeft != nullptr)
			stLeft->Print(nSpace + 2);
		printf("%s Right:\n", chs);
		if (stRight != nullptr)
			stRight->Print(nSpace + 2);
		DELETE_CHS;
	}
};

// Relational expression structure
struct stRelational : stExpression
{
public:
	// Left side
	stExpression* stLeft;
	// Right side
	stExpression* stRight;
	// Relational type
	CLexer::eLexEnum eType;

	stRelational()
		: stLeft(nullptr), stRight(nullptr), eType(CLexer::eLexEnum::Unknown)
	{}

	~stRelational()
	{
		DeletePtr<stExpression>(stLeft);
		DeletePtr<stExpression>(stRight);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sRelational(%s):\n", chs, CLexer::FindLexToString(eType).c_str());
		printf("%s Left:\n", chs);
		if (stLeft != nullptr)
			stLeft->Print(nSpace + 2);
		printf("%s Right:\n", chs);
		if (stRight != nullptr)
			stRight->Print(nSpace + 2);
		DELETE_CHS;
	}
};

// Arithmetic expression structure
struct stArithmetic : stExpression
{
public:
	// Left side
	stExpression* stLeft;
	// Right side
	stExpression* stRight;
	// Arithmetic type
	CLexer::eLexEnum eType;

	stArithmetic()
		: stLeft(nullptr), stRight(nullptr), eType(CLexer::eLexEnum::Unknown)
	{}

	~stArithmetic()
	{
		DeletePtr<stExpression>(stLeft);
		DeletePtr<stExpression>(stRight);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sArithmetic(%s):\n", chs, CLexer::FindLexToString(eType).c_str());
		printf("%s Left:\n", chs);
		if (stLeft != nullptr)
			stLeft->Print(nSpace + 2);
		printf("%s Right:\n", chs);
		if (stRight != nullptr)
			stRight->Print(nSpace + 2);
		DELETE_CHS;
	}
};

// Unary expression structure
struct stUnary : stExpression
{
public:
	// Subsituation expression
	stExpression* stSubExp;
	// Unary type
	CLexer::eLexEnum eType;

	stUnary()
		: stSubExp(nullptr), eType(CLexer::eLexEnum::Unknown)
	{}

	~stUnary()
	{
		DeletePtr<stExpression>(stSubExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);
		printf("%sUnary(%s):\n", chs, CLexer::FindLexToString(eType).c_str());
		printf("%s Subsituation expression:\n", chs);
		if (stSubExp != nullptr)
			stSubExp->Print(nSpace + 2);
		DELETE_CHS;
	}
};

// Get variable data expression structure
struct stGetVariable : stExpression
{
public:
	// Variable name
	std::string strName;

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);

		DELETE_CHS;
	}
};

// Set variable data expression structure
struct stSetVariable : stExpression
{
public:
	// Variable name
	std::string strName;
	// Initialize expression
	stExpression* stInitExp;

	stSetVariable()
		: strName(""), stInitExp(nullptr)
	{}

	~stSetVariable()
	{
		DeletePtr<stExpression>(stInitExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);

		DELETE_CHS;
	}
};

// Get array or container element data expression structure
struct stGetElement : stExpression
{
public:
	// Array or container expression
	stExpression* stMemsExp;
	// Index expression
	stExpression* stIndexExp;

	stGetElement()
		: stMemsExp(nullptr), stIndexExp(nullptr)
	{}

	~stGetElement()
	{
		DeletePtr<stExpression>(stMemsExp);
		DeletePtr<stExpression>(stIndexExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);

		DELETE_CHS;
	}
};

// Set array or container element data expression structure
struct stSetElement : stExpression
{
public:
	// Array or container expression
	stExpression* stMemsExp;
	// Index expression
	stExpression* stIndexExp;
	// Initialize expression
	stExpression* stInitExp;

	stSetElement()
		: stMemsExp(nullptr), stIndexExp(nullptr), stInitExp(nullptr)
	{}

	~stSetElement()
	{
		DeletePtr<stExpression>(stMemsExp);
		DeletePtr<stExpression>(stIndexExp);
		DeletePtr<stExpression>(stInitExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);

		DELETE_CHS;
	}
};

// Call function expression structure
struct stCallFunc : stExpression
{
public:
	// Subsituation expression
	stExpression* stSubExp;
	// Function arguments
	std::vector<stExpression*> vArgsExp;

	stCallFunc()
		: stSubExp(nullptr)
	{}

	~stCallFunc()
	{
		DeleteVectorPtrArgs<stExpression>(vArgsExp);
	}

	void Print(int nSpace) override
	{
		CREATE_CHS(nSpace);

		DELETE_CHS;
	}
};

// Program structure
struct stProgram
{
public:
	std::vector<stFunction*> vFunc;

	~stProgram()
	{
		DeleteVectorPtrArgs<stFunction>(vFunc);
	}

	void Print()
	{
		std::vector<stFunction*>::iterator iter = vFunc.begin();

		while (iter != vFunc.begin())
		{
			(*iter)->Print(0);

			++iter;
		}
	}
};
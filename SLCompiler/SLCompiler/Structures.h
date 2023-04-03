#pragma once
#include <string>
#include <vector>
#include "Lexer.h"

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

// Statement structure
struct stStatement
{

};

// Expression structure
struct stExpression
{

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
		: strName(""), stExp(nullptr), eType(CLexer::eLexEnum::Void)
	{}

	~stVariable()
	{
		DeletePtr<stExpression>(stExp);
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
};

// If statement structure
struct stIf : stStatement
{
public:
	// If condition statements
	std::vector<stStatement*> stCondStm;
	// If expression block
	std::vector<std::vector<stStatement*>> vIfBlock;
	// Else expression block
	std::vector<stStatement*> vElseBlock;

	stIf()
	{}

	~stIf()
	{
		DeleteVectorPtrArgs<stStatement>(stCondStm);
		DeleteVectorPtrArgs<stStatement>(vElseBlock);

		int nSize = (int)vIfBlock.size();
		if (nSize == 0)
			return;
		for (int i = 0; i < nSize; ++i)
			DeleteVectorPtrArgs<stStatement>(vIfBlock[i]);
	}
};

// Switch statement structure
struct stSwitch : stStatement
{
public:
	// Switch variable (Init-statement)
	stVariable* stVar;
	// Switch case, default statements block
	std::vector<std::vector<stStatement*>> vCaseBlock;

	stSwitch()
		: stVar(nullptr)
	{}

	~stSwitch()
	{
		DeletePtr<stVariable>(stVar);

		int nSize = (int)vCaseBlock.size();
		if (nSize == 0)
			return;
		for (int i = 0; i < nSize; ++i)
			DeleteVectorPtrArgs<stStatement>(vCaseBlock[i]);
	}
};

// Break statement structure
struct stBreak : stStatement
{

};

// Continue statement Structure
struct stContinue : stStatement
{

};

// Print statement structure
struct stPrint : stStatement
{
public:
	// Print arguments
	std::vector<stExpression*> stArgs;
};

// Null data type structure
struct stNullData : stExpression
{

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
};

// Get variable data expression structure
struct stGetVariable : stExpression
{
public:
	// Variable name
	std::string strName;
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
};
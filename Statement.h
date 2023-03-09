//************************************************************************** 
//
// Final Programming Excerise 
// CLASS: COSC 2336.S01
// AUTHOR: TYLER ECHOLS 
// DAtE: Dec 2017  
//
//**************************************************************************
#ifndef STATEMENT_H
#define STATEMENT_H

#include <iostream>
#include <fstream>

using namespace std;

#include "WCS_Pointer.h"
#include "WCS_String.h"
#include "Operator.h"
#include "Variable.h"
#include "Token.h"
#include "MathTree.h"


class Statement
{
public:
	typedef unsigned char			UChar;

	enum	Variables {
		NumVariables = 100
	};

	enum	Exception {
		StatementFileNotOpen
	};

	//"Token" Types
	enum	StatementType {
		EndOfInputStatement,
		UnknownStatement,
		VariableStatement,
		ConstantStatement,
		KeywordEvalStatement,
		KeywordExpStatement,
		KeywordValueStatement,
		OperatorPlusStatement,
		OperatorMinusStatement,
		OperatorAsteriskStatement,
		OperatorSlashStatement,
		SymbolOpenParenStatement,
		SymbolCloseParenStatement,
		SymbolSemiColonStatement,

		NumStatements
	};

	Statement();
	Statement(StatementType, int);
	~Statement();
	Statement &						Build();
	static	void					CloseFile();
	void OpenFile(const WCS_String &);
	StatementType					GetType() const;
	//static	void					OpenFile(const WCS_String &);
	static	void					OpenKeyboard();
	void							OpenTokenFile(const WCS_String &);
	void							CloseTokenFile();

private:

	fstream FileName;

	enum	InputType
	{
		EndOfInputToken,
		UnknownToken,
		VariableToken,
		ConstantToken,
		KeywordEvalToken,
		KeywordExpToken,
		KeywordValueToken,
		OperatorPlusToken,
		OperatorMinusToken,
		OperatorAsteriskToken,
		OperatorSlashToken,
		SymbolOpenParenToken,
		SymbolCloseParenToken,
		SymbolSemiColonToken,

		NumInputs
	};

	enum	StatementState
	{
		StatementComplete = -1,
		StartStatement,
		StartEXP,
		AfterOperand,
		AfterOperator,
		AfterEVAL,
		AfterVariable,
		StartVALUE,
		AfterVariableAfterVALUE,
		AfterVALUEVariableConstant,
		GoToSemiColon,
		AfterUnaryOperator,

		NumStates
	};
	typedef int						State;
	typedef StatementState(*StatementFunc)	(Statement &);

	InputType						GetInputChar();

	static	void					PutBackLastChar();

	void							SetType(StatementType);


	static	int						ConvertLastChar();
	//	***** State Functions *****

	static	StatementState				InvalidExpression(Statement &);
	static	StatementState				GotEXP(Statement &);
	static	StatementState				GotEVAL(Statement &);
	static	StatementState				GotUnaryOperatorPLUS(Statement &);
	static	StatementState				GotUnaryOperatorMINUS(Statement &);
	static	StatementState				GotBinaryOperatorPM(Statement &);
	static	StatementState				GotOpenPren(Statement &);
	static	StatementState				GotClosePren(Statement &);
	static	StatementState				GotVariable(Statement &);
	static	StatementState				GotConstant(Statement &);
	static	StatementState				GotVALUE(Statement &);
	static	StatementState				GotVariableAfterVALUE(Statement &);
	static	StatementState				GotConstantAFterVALUEVarialbe(Statement &);
	static	StatementState				InValidStatement(Statement &);
	static	StatementState				ValidEVAL(Statement &);
	static	StatementState				ValidEXP(Statement &);
	static	StatementState				ValidVALUE(Statement &);
	static	StatementState				GotBinaryOperatorMD(Statement &);
	static	StatementState				GotEOF(Statement &);
	static	StatementState				GotPlusAfterVariableValue(Statement &);
	static	StatementState				GotMinusAfterVariableValue(Statement &);
	static	InputType				InputTable[];
	static	UChar					LastChar;
	static	ifstream				SourceFile;
	static	StatementFunc			StateFunction[];
	static	int						StateTable[NumInputs][NumStates];
	static	Variable				VariableArray[NumVariables];

	// ***** Data members *****
	Token					MyToken;
	MathTree				MyTree;
	StatementType			Type;
	Token					TempVariableToken;
	int						TempUnaryTracker = 1;
};

inline Statement::Statement()
{
}



//
//inline Statement::Statement(const WCS_Pointer<Statement> & T)
//{
//	Init((*T).Type, (*T).Which);
//}

inline Statement::Statement(StatementType NewType, int NewValue)
{
}

inline Statement::~Statement()
{
}

inline void Statement::CloseFile()
{
	SourceFile.close();
}

inline int Statement::ConvertLastChar()
{
	return LastChar - '0';
}

inline void Statement::OpenFile(const WCS_String & F)
{
	SourceFile.open(static_cast <const char *> (F));
	if (!SourceFile.is_open())
	{
		cout << "File did not open" << endl;
		exit(0);
	}
	else;
}
inline Statement::StatementType Statement::GetType() const
{
	return Type;
}
//inline void Statement::OpenFile(const WCS_String & FileName)
//{
//	SourceFile.open((const char *)FileName);
//}

inline void Statement::OpenKeyboard()
{
	//	SourceFile.attach (0);  // no longer available under new release of iostream
}


inline Statement::InputType Statement::GetInputChar()
{
	return InputTable[LastChar = (UChar)SourceFile.get()];
}


inline void Statement::PutBackLastChar()
{
	SourceFile.putback(LastChar);
}


inline void Statement::SetType(StatementType T)
{
	Type = T;
}

inline void Statement::OpenTokenFile(const WCS_String & s)
{
	MyToken.OpenFile(s);
}

inline void Statement::CloseTokenFile()
{
	MyToken.CloseFile();
}


#endif

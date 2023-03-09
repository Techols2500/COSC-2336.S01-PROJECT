//************************************************************************** 
//
// Final Programming Excerise 
// CLASS: COSC 2336.S01
// AUTHOR: TYLER ECHOLS 
// DAtE: Dec 2017  
//
//**************************************************************************
#ifndef TOKEN_H
#define TOKEN_H

#include <fstream>
#include <iostream>

using namespace std;

#include "WCS_String.h"
#include "Operator.h"
#include "Variable.h"

class Token
{
public:
	//		**********	enums		**********
	enum	Variables
	{
		NumVariables = 100
	};
	enum	Exception
	{
		TokenFileNotOpen
	};
	enum	TokenType
	{
		EndOfInputToken,      //0
		UnknownToken,         //1
		VariableToken,        //2
		ConstantToken,        //3
		KeywordEvalToken,     //4
		KeywordExpToken,      //5
		KeywordValueToken,    //6
		OperatorPlusToken,    //7
		OperatorMinusToken,   //8
		OperatorAsteriskToken,//9
		OperatorSlashToken,   //10
		SymbolOpenParenToken, //11
		SymbolCloseParenToken,//12
		SymbolSemiColonToken, //13

		NumTokens
	};
	//		**********	typedefs	**********
	typedef unsigned char			UChar;
	//		**********	functions	**********
	Token();
	explicit				Token(const Token &);
	explicit				Token(TokenType, int);
	~Token();
	Token &					Build();
	static	void					CloseFile();
	Operator::Precedence			GetPrecedence() const;
	TokenType				GetType() const;
	Variable::ValueType		GetValue() const;
	int						GetWhich() const;
	static	void					OpenFile(const WCS_String &);
	void					SetPrecedence(Operator::Precedence);
	void					SetValue(Variable::ValueType);
	Token &					operator=		(const Token &);
	void					SetWhich(int);
private:
	//		**********	enums		**********
	enum	InputType
	{
		InputUnknown,
		InputA,
		InputE,
		InputL,
		InputP,
		InputU,
		InputV,
		InputX,
		Input0,
		Input1,
		Input2,
		Input3,
		Input4,
		Input5,
		Input6,
		Input7,
		Input8,
		Input9,
		InputPlus,
		InputMinus,
		InputAsterisk,
		InputSlash,
		InputOpenParen,
		InputCloseParen,
		InputSemiColon,
		InputSpace,
		EndOfFile,

		NumInputs
	};
	enum	TokenState
	{
		TokenComplete = -1,
		StartGettingToken,
		AfterE,
		AfterV,
		AfterEV,
		AfterEVA,
		AfterVA,
		AfterVAL,
		AfterVALU,
		AfterEX,
		AfterVDigit,
		AfterDigit,
		AfterVDigitDigit,
		NumStates
	};
	//		**********	typedefs	**********
	typedef int						State;
	typedef TokenState(*TokenFunc)		(Token &);
	//	**********	functions	**********
	InputType				GetInputChar();
	void					Init(TokenType, int);
	void					SetType(TokenType);

	static	int						ConvertLastChar();
	static	void					PutBackLastChar();



	static	TokenState				GotInvalidInput(Token &);
	static	TokenState				NotYetWritten(Token &);
	static	TokenState				GotE(Token &);
	static	TokenState				GotEV(Token &);
	static	TokenState				GotEVA(Token &);
	static	TokenState				GotEVAL(Token &);
	static	TokenState				GotV(Token &);
	static	TokenState				GotVA(Token &);
	static	TokenState				GotVAL(Token &);
	static	TokenState				GotVALU(Token &);
	static	TokenState				GotVALUE(Token &);
	static	TokenState				GotEX(Token &);
	static	TokenState				GotEXP(Token &);
	static	TokenState				GotEndOfFile(Token &);
	static	TokenState				GotUnexpectedEOF(Token &);
	static	TokenState				StartedTokenWithSpace(Token &);
	static	TokenState				GotPlus(Token &);
	static	TokenState				GotMinus(Token &);
	static	TokenState				GotAsterisk(Token &);
	static	TokenState				GotSlash(Token &);
	static	TokenState				GotOpenParenth(Token &);
	static	TokenState				GotCloseParenth(Token &);
	static	TokenState				GotSemiColon(Token &);
	static	TokenState				GotSpace(Token &);;
	static	TokenState				GotDigitAfterV(Token &);
	static	TokenState				GotDigitAfterVDigit(Token &);
	static	TokenState				GotFirstDigit(Token &);
	static	TokenState				GotFollowingDigit(Token &);
	static	TokenState				GotNonDigitFollowingDigit(Token &);
	static	TokenState				GotVDigitDigit(Token &);




	static	InputType				InputTable[];
	static	UChar					LastChar;
	static	ifstream				SourceFile;
	static	TokenFunc				StateFunction[];
	static	UChar					StateTable[NumInputs][NumStates];
	static	Variable				VariableArray[NumVariables];
	//	**********	data		**********
	TokenType				Type;
	union
	{
		int						Which;
		Operator::Precedence	Precedence;
		Variable::ValueType		Value;
	};
};

inline Token::Token()
{
	Init(EndOfInputToken, 0);
}

inline Token::Token(const Token & T)
{
	Init(T.Type, T.Which);
}

inline Token::Token(TokenType NewType, int NewValue)
{
	Init(NewType, NewValue);
}

inline Token::~Token()
{
}

inline void Token::CloseFile()
{
	SourceFile.close();
	cout << "File has been closed" << endl;
}

inline Operator::Precedence Token::GetPrecedence() const
{
	return Precedence;
}

inline Token::TokenType Token::GetType() const
{
	return Type;
}

inline Variable::ValueType Token::GetValue() const
{
	return (Type == VariableToken) ? VariableArray[Which].GetValue() : Value;
}

inline int Token::GetWhich() const
{
	return Which;
}

inline void Token::OpenFile(const WCS_String & FileName)
{
	SourceFile.open(static_cast <const char *> (FileName));
	if (!SourceFile.is_open())
	{
		cout << "File did not open" << endl;
		exit(0);
	}
	else;
}

inline void Token::SetValue(Variable::ValueType V)
{
	if (Type == VariableToken)
		VariableArray[Which].SetValue(V);
	else
		Value = V;
}

inline Token & Token::operator= (const Token & T)
{
	if (this != &T)
		Init(T.Type, T.Which);
	else;
	return *this;
}

inline Token::InputType Token::GetInputChar()
{
	return InputTable[LastChar = static_cast <UChar> (SourceFile.get())];
}

inline void Token::Init(TokenType T, int V)
{
	Type = T;
	Value = V;
}

inline void Token::PutBackLastChar()
{
	SourceFile.putback(LastChar);
}

inline void Token::SetPrecedence(Operator::Precedence P)
{
	Precedence = P;
}

inline void Token::SetType(TokenType T)
{
	Type = T;
}

inline void Token::SetWhich(int W)
{
	Which = W;
}
inline int Token::ConvertLastChar()
{
	return LastChar - '0';
}


#endif

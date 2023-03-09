//************************************************************************** 
//
// Final Programming Excerise 
// CLASS: COSC 2336.S01
// AUTHOR: TYLER ECHOLS 
// DAtE: Dec 2017  
//
//**************************************************************************
#define Statement_CPP

#include "Statement.h"
#include "Token.h"

//**********  Static Variables from Statement Class **********

Statement::UChar Statement::LastChar;

ifstream Statement::SourceFile;

Statement::StatementFunc Statement::StateFunction[] = {
	InvalidExpression,				//  0
	GotEXP,							//  1
	GotEVAL,						//  2
	GotUnaryOperatorPLUS,			//  3
	GotBinaryOperatorPM,			//  4
	GotOpenPren,					//  5
	GotClosePren,					//	6
	GotVariable,					//  7
	GotConstant,					//  8
	GotVALUE,						//  9
	GotVariableAfterVALUE,			//  10
	GotConstantAFterVALUEVarialbe,  //  11
	InValidStatement,				//  12
	ValidEVAL,						//  13
	ValidEXP,						//  14
	ValidVALUE,						//  15
	GotBinaryOperatorMD,			//  16
	GotEOF,							//  17
	GotPlusAfterVariableValue,		//  18
	GotMinusAfterVariableValue,		//  19
	GotUnaryOperatorMINUS			//  20
};

int Statement::StateTable[NumInputs][NumStates] = {

	//						
	//							Start		Start	After	After		After	After		Start	VariableAfter	AfterVALUE	 	GoTo		After
	//							Statement	EXP		Operand	Operator	EVAL	Variable	VALUE	VALUE			VariableConst	SemiColon	UnnaryOperator
	/*EndOfInputToken,	*/{ 17,		0,		0,	    0,	        0,		0,			0,		 0,				0,				17,			0, },
	/*UnknownToken,	*/{ 0,		0,		0,	    0,	        0,		0,			0,		 0,				0,				0,			0, },
	/*VariableToken,	*/{ 0,		7,		0,	    7,	        0,		0,			10,		 0,				0,				0,			7, },
	/*ConstantToken,	*/{ 0,		8,		0,	    8,	        0,		0,			0,		 11,			0,				0,			8, },
	/*KeywordEvalToken,	*/{ 2,		0,		0,	    0,	        0,		0,			0,		 0,				0,				0,			0, },
	/*KeywordExpToken,	*/{ 1,		0,		0,	    0,	        0,		0,			0,		 0,				0,				0,			0, },
	/*KeywordValueToken,*/{ 9,		0,		0,	    0,	        0,		0,			0,		 0,				0,				0,			0, },
	/*OperatorPlusToken,*/{ 0,		3,		4,	    3,	        0,		4,			0,		 18,			0,				0,			3, },
	/*OperatorMinusToken,*/{ 0,		20,		4,	    20,	        0,		4,			0,		 19,			0,				0,			20, },
	/*OperatorAsteriskToken,*/{ 0,		0,		16,	    0,	        0,		16,			0,		 0,				0,				0,			0, },
	/*OperatorSlashToken,	*/{ 0,		0,		16,	    0,	        0,		16,			0,		 0,				0,				0,			0, },
	/*SymbolOpenParenToken,	*/{ 0,		5,		0,	    5,	        0,		0,			0,		 0,				0,				0,			5, },
	/*SymbolCloseParenToken,*/{ 0,		0,		6,	    0,	        0,		6,			0,		 0,				0,				0,			0, },
	/*SymbolSemiColonToken,	*/{ 0,		0,		14,	    0,	        13,		14,			0,		 0,				15,				12,			0, },

};

Variable Statement::VariableArray[NumVariables];

//********** Methods from Statement Class **********

Statement & Statement::Build()
{

	StatementState CurrentState(StartStatement);
	SetType(UnknownStatement);
	do {
		MyToken.Build();
		CurrentState = (StateFunction[StateTable[MyToken.GetType()][CurrentState]]) (*this);
		//				CurrentState = (this->*StateFunction [StateTable [GetInputChar ()] [CurrentState] ]) ();
	} while (CurrentState != StatementComplete);
	return *this;

}
Statement::StatementState Statement::InvalidExpression(Statement &)
{
	return GoToSemiColon;
}
Statement::StatementState Statement::GotEXP(Statement & T)
{
	T.MyTree.DeleteTree();
	cout << "\n\nThe tree has been deleted after GETTING VALID EXP statement" << endl;
	return StartEXP;
}

Statement::StatementState Statement::GotEVAL(Statement &)
{

	return AfterEVAL;
}

Statement::StatementState Statement::GotUnaryOperatorPLUS(Statement & T)
{
	T.TempUnaryTracker *= 1;
	return AfterUnaryOperator;
}

Statement::StatementState Statement::GotUnaryOperatorMINUS(Statement & T)
{
	T.TempUnaryTracker *= -1;
	return AfterUnaryOperator;
}

Statement::StatementState Statement::GotBinaryOperatorPM(Statement & T)
{
	T.MyToken.SetPrecedence((Operator::Precedence) (Operator::BinaryPlusMinusPrecedence + (Operator::ParenthesesPrecedence * T.MyTree.NumOpenPeren)));

	T.MyTree.InsertBinaryOperator(T.MyToken);
	return AfterOperator;
}


Statement::StatementState Statement::GotEOF(Statement & T)
{


	// Make sure to close file.

	T.CloseTokenFile();

	exit(0);
}


Statement::StatementState Statement::GotBinaryOperatorMD(Statement & T)
{
	T.MyToken.SetPrecedence((Operator::Precedence) (Operator::MultiplyDividePrecedence + (Operator::ParenthesesPrecedence * T.MyTree.NumOpenPeren)));

	T.MyTree.InsertBinaryOperator(T.MyToken);
	return AfterOperator;
}

Statement::StatementState Statement::GotOpenPren(Statement & T)
{
	T.MyTree.NumOpenPeren++;
	cout << "Got an open parenthesis" << endl;
	return AfterOperator;

}

Statement::StatementState Statement::GotClosePren(Statement & T)
{
	if (T.MyTree.NumOpenPeren != 1)
	{
		T.MyTree.NumOpenPeren--;
		cout << "Got a close parenthesis" << endl;
		return AfterOperand;
	}
	else
	{
		T.MyTree.NumOpenPeren = 1;
		cout << "Got an invalid number of ), please check the statement." << endl;
		T.MyTree.DeleteTree();
		cout << "The tree has been deleted after getting an invalid number of )." << endl;

		return StartStatement;
	}

}

Statement::StatementState Statement::GotVariable(Statement & T)
{
	T.TempUnaryTracker = 1;
	T.MyTree.InsertOperand(T.MyToken);
	return AfterVariable;
}

Statement::StatementState Statement::GotConstant(Statement & T)
{
	T.MyToken.SetValue(T.MyToken.GetValue() * T.TempUnaryTracker);
	T.MyTree.InsertOperand(T.MyToken);
	T.TempUnaryTracker = 1;
	return AfterVariable;
}

Statement::StatementState Statement::GotVALUE(Statement &)
{
	return StartVALUE;
}
Statement::StatementState Statement::GotVariableAfterVALUE(Statement & T)
{

	T.TempVariableToken = T.MyToken;

	return AfterVariableAfterVALUE;
}
Statement::StatementState Statement::GotConstantAFterVALUEVarialbe(Statement & T)
{
	T.TempVariableToken.SetValue(T.MyToken.GetWhich() * T.TempUnaryTracker);
	T.TempUnaryTracker = 1;
	return AfterVALUEVariableConstant;
}



Statement::StatementState Statement::GotPlusAfterVariableValue(Statement & T)
{
	T.TempUnaryTracker *= 1;
	return AfterVariableAfterVALUE;
}
Statement::StatementState Statement::GotMinusAfterVariableValue(Statement & T)
{
	T.TempUnaryTracker *= -1;
	return AfterVariableAfterVALUE;
}


Statement::StatementState Statement::InValidStatement(Statement & T)
{
	T.MyTree.DeleteTree();
	cout << "INVALID STATEMENT" << endl;
	cout << "The tree has been deleted after INVALID STATEMENT" << endl;
	return StartStatement;
}

Statement::StatementState Statement::ValidEVAL(Statement & T)
{
	if (T.MyTree.NumOpenPeren == 1)
	{
		try
		{
			T.MyTree.Evaluate();
		}
		catch (int)
		{
			// DO NOT CHANGE, IT LOOKS OFF, BUT IT WORKS.***********************************************************
			T.MyTree.DeleteTree();
			cout << "The tree has been deleted after DIVIDING BY 0" << endl;
			T.MyTree.NumOpenPeren = 1;
		}
	}
	else
	{
		cout << "Your () are not correct, please correct them." << endl;
		T.MyTree.DeleteTree();
		cout << "The tree has been deleted after INCORRECT NUMBER OF ()" << endl;
		T.MyTree.NumOpenPeren = 1;
	}
	return StartStatement;
}

Statement::StatementState Statement::ValidEXP(Statement & T)
{
	cout << "You have put in a valid EXP command" << endl;
	return StartStatement;
}

Statement::StatementState Statement::ValidVALUE(Statement & T)
{
	cout << "You have put in a valid VALUE command" << endl;
	return StartStatement;
}

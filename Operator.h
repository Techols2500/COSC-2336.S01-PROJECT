//************************************************************************** 
//
// Final Programming Excerise 
// CLASS: COSC 2336.S01
// AUTHOR: TYLER ECHOLS 
// DAtE: Dec 2017  
//
//**************************************************************************
#ifndef OPERATOR_H
#define OPERATOR_H

class Operator
{
public:
	enum Precedence
	{
		BinaryPlusMinusPrecedence,
		MultiplyDividePrecedence,
		UnaryPlusMinusPrecedence,
		ParenthesesPrecedence,
	};
	Operator();
	Operator(const Operator &);
	~Operator();
	int BinaryPlusMinusPrecedenceCHANGE = 0;
	int MultiplyDividePrecedenceCHANGE = 0;
	int UnaryPlusMinusPrecedenceCHANGE = 0;

private:


	Operator &	operator =	(const Operator &);
};


#endif

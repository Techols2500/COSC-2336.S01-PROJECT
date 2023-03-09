//************************************************************************** 
//
// Final Programming Excerise 
// CLASS: COSC 2336.S01
// AUTHOR: TYLER ECHOLS 
// DAtE: Dec 2017  
//
//**************************************************************************
#include "MathTree.h"

MathTree::Node::Node(const Token & S) : Toke(S)
{
}

MathTree::MathTree()
{
}

MathTree::~MathTree()
{
	DeleteTree();
	cout << "The tree has been deleted" << endl;
	NumOpenPeren = 1;
}

void MathTree::DeleteTree()
{
	if (pRoot.Exists())
	{

		WCS_Pointer <Node> pNode(GetLeftMostOperator(pRoot));
		WCS_Pointer <Node> pTemp(pRoot);

		while (pNode.Exists())
		{
			if (!(*pNode).LeftSideDone)
			{

				(*(*pNode).pLeft).pLeft.SetToNull();
				(*(*pNode).pLeft).pRight.SetToNull();
				(*(*pNode).pLeft).pParent.SetToNull();
				(*pNode).LeftSideDone = true;

				if ((*pNode).pRight.Exists())
				{
					pNode = GetLeftMostOperator((*pNode).pRight);

				}
			}
			else
			{
				pNode = (*pNode).pParent;
			}

			if (pNode = pRoot)
			{
				pNode.SetToNull();
				pRoot.SetToNull();
				pLastOperator.SetToNull();
			}
		}
	}

	else
	{
	}
}

Variable::ValueType MathTree::Evaluate()
{
	if (pRoot.Exists())
	{
		WCS_Pointer <Node>	pNode(GetLeftMostOperator(pRoot));

		while (pNode.Exists())
		{
			if (!(*pNode).LeftSideDone)
			{
				(*pNode).Toke.SetValue((*(*pNode).pLeft).Toke.GetValue());
				(*pNode).LeftSideDone = true;
				pNode = GetLeftMostOperator((*pNode).pRight);
			}
			else
			{
				switch ((*pNode).Toke.GetType())
				{
				case Token::OperatorPlusToken:
					cout << (*pNode).Toke.GetValue() << "+" << (*(*pNode).pRight).Toke.GetValue() << endl;
					(*pNode).Toke.SetValue((*pNode).Toke.GetValue() + (*(*pNode).pRight).Toke.GetValue());
					break;
				case Token::OperatorMinusToken:
					// for subtraction
					cout << (*pNode).Toke.GetValue() << "-" << (*(*pNode).pRight).Toke.GetValue() << endl;
					(*pNode).Toke.SetValue((*pNode).Toke.GetValue() - (*(*pNode).pRight).Toke.GetValue());
					break;
				case Token::OperatorAsteriskToken:
					// for multiplication
					cout << (*pNode).Toke.GetValue() << "*" << (*(*pNode).pRight).Toke.GetValue() << endl;
					(*pNode).Toke.SetValue((*pNode).Toke.GetValue() * (*(*pNode).pRight).Toke.GetValue());
					break;
				case Token::OperatorSlashToken:
					// for division
					cout << (*pNode).Toke.GetValue() << "/" << (*(*pNode).pRight).Toke.GetValue() << endl;

					if ((*(*pNode).pRight).Toke.GetValue() != 0)
					{
						(*pNode).Toke.SetValue((*pNode).Toke.GetValue() / (*(*pNode).pRight).Toke.GetValue());
						break;
					}
					else
					{
						throw 666;

					}

				}
				pNode = (*pNode).pParent;

			}
		}
		cout << "The result is " << (*pRoot).Toke.GetValue() << endl;
		return (*pRoot).Toke.GetValue();
	}
	else
		cout << "There is no tree to be evaluated" << endl;
	return 0;
}

WCS_Pointer <MathTree::Node> & MathTree::GetLeftMostOperator(const WCS_Pointer <Node> & pStart)
{
	WCS_Pointer <Node> Ptr(pStart);

	while ((*Ptr).pLeft.Exists())
	{
		(*Ptr).LeftSideDone = false;
		Ptr = (*Ptr).pLeft;
	}
	return (*Ptr).pParent;
}

MathTree::Result MathTree::InsertBinaryOperator(const Token & Toke)
{
	WCS_Pointer <Node>	pNode(new Node(Toke));

	if (pLastOperator.DoesNotExist())
	{
		(*pRoot).pParent = pNode;
		(*pNode).pLeft = pRoot;
		pRoot = pNode;
		pLastOperator = pNode;
		return OK;
	}
	else
	{
		if (Toke.GetPrecedence() > (*pLastOperator).Toke.GetPrecedence())
		{
			(*pNode).pLeft = (*pLastOperator).pRight;
			(*(*pNode).pLeft).pParent = pNode;
			(*pLastOperator).pRight = pNode;
			(*pNode).pParent = pLastOperator;
			pLastOperator = pNode;
			return OK;
		}
		else
		{
			while (pLastOperator && (Toke.GetPrecedence() <= (*pLastOperator).Toke.GetPrecedence()))
				pLastOperator = (*pLastOperator).pParent;
			if (pLastOperator.Exists())
			{
				(*pNode).pLeft = (*pLastOperator).pRight;
				(*pNode).pParent = pLastOperator;
				(*pLastOperator).pRight = pNode;
				(*(*pNode).pRight).pParent = pNode;
			}
			else
			{
				(*pNode).pLeft = pRoot;
				(*pRoot).pParent = pNode;
				pRoot = pNode;
			}
			pLastOperator = pNode;
		}
	}
}


MathTree::Result MathTree::InsertOperand(const Token & Toke)
{
	WCS_Pointer <Node>	pNode(new Node(Toke));

	if (pRoot.DoesNotExist())
		pRoot = pNode;
	else
	{
		(*pLastOperator).pRight = pNode;
		(*pNode).pParent = pLastOperator;
	}
	return OK;
}

MathTree::Result MathTree::InsertUnaryOperator(const Token & Toke)
{
	Token				Temp(Token::ConstantToken, 0);

	InsertOperand(Temp);
	return InsertBinaryOperator(Toke);
}

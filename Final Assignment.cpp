//************************************************************************** 
//
// Final Programming Excerise 
// CLASS: COSC 2336.S01
// AUTHOR: TYLER ECHOLS 
// DAtE: Dec 2017  
//
//**************************************************************************
#include <iostream>
#include "Statement.h" 
#include "MathTree.h" 

using namespace std; 

void main(int argc, char* argv[])
{

	Statement S;
	S.OpenTokenFile(WCS_String("Test File for Semester Project.txt"));


	S.Build();

	S.CloseTokenFile();
}


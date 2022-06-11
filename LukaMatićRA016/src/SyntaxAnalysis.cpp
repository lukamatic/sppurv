// Autor: Luka Matić Datum: 12.06.2020.

#include <iostream>
#include <iomanip>

#include "SyntaxAnalysis.h"

using namespace std;


SyntaxAnalysis::SyntaxAnalysis(LexicalAnalysis& lex)
	: lexicalAnalysis(lex), errorFound(false), tokenIterator(lexicalAnalysis.getTokenList().begin())
{
}


bool SyntaxAnalysis::Do()
{
	cout << "\nStarting Syntax analysis...\n" << endl;

	currentToken = getNextToken();

	//TO DO: Call function for the starting non-terminal symbol
	Q();

	return !errorFound;
}


void SyntaxAnalysis::printSyntaxError(Token token)
{
	cout << endl << "Syntax error! Token: " << token.getValue() << " unexpected" << endl;
}

void SyntaxAnalysis::eat(TokenType t)
{
	if (errorFound == false)
	{
		if (currentToken.getType() == t)
		{
			currentToken.printTokenInfo();
			currentToken = getNextToken();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
}

Token SyntaxAnalysis::getNextToken()
{
	if (tokenIterator == --lexicalAnalysis.getTokenList().end())
		return *tokenIterator;

	return *tokenIterator++;
}

void SyntaxAnalysis::Q()
{
	S();

	if (currentToken.getType() == T_SEMI_COL)
		eat(T_SEMI_COL);
	else
	{
		printSyntaxError(currentToken);
		errorFound = true;
	}

	L();
}


void SyntaxAnalysis::S()
{
	if (errorFound)
		return;

	if (currentToken.getType() == T_MEM)
	{
		eat(T_MEM);

		if (currentToken.getType() == T_M_ID)
		{
			eat(T_M_ID);

			if (currentToken.getType() == T_NUM)
			{
				eat(T_NUM);
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_REG)
	{
		eat(T_REG);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_FUNC)
	{
		eat(T_FUNC);

		if (currentToken.getType() == T_ID)
		{
			eat(T_ID);
		}
	}
	else if (currentToken.getType() == T_ID)
	{
		eat(T_ID);

		if (currentToken.getType() == T_COL)
		{
			eat(T_COL);

			E();
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else
		E();
}


void SyntaxAnalysis::L()
{
	if (errorFound)
		return;

	if (currentToken.getType() == T_END_OF_FILE)
 		eat(T_END_OF_FILE);
	else
		Q();
}


void SyntaxAnalysis::E()
{
	if (errorFound)
		return;

	if (currentToken.getType() == T_ADD)
	{
		eat(T_ADD);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA); 
				
				if (currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);

					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);

						if (currentToken.getType() == T_R_ID)
						{
							eat(T_R_ID);
						}
						else
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_ADDI)
	{
		eat(T_ADDI);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);

					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);

						if (currentToken.getType() == T_NUM)
						{
							eat(T_NUM);
						}
						else
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_SUB)
	{
		eat(T_SUB);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);

					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);

						if (currentToken.getType() == T_R_ID)
						{
							eat(T_R_ID);
						}
						else
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_LA)
	{
		eat(T_LA);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_M_ID)
				{
					eat(T_M_ID);
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_LW)
	{
		eat(T_LW);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_NUM)
				{
					eat(T_NUM);

					if (currentToken.getType() == T_L_PARENT)
					{
						eat(T_L_PARENT);

						if (currentToken.getType() == T_R_ID)
						{
							eat(T_R_ID);

							if (currentToken.getType() == T_R_PARENT)
							{
								eat(T_R_PARENT);
							}
							else
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_LI)
	{
		eat(T_LI);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_NUM)
				{
					eat(T_NUM);
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
	}
	else if (currentToken.getType() == T_SW)
	{
		eat(T_SW);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_NUM)
				{
					eat(T_NUM);

					if (currentToken.getType() == T_L_PARENT)
					{
						eat(T_L_PARENT);

						if (currentToken.getType() == T_R_ID)
						{
							eat(T_R_ID);

							if (currentToken.getType() == T_R_PARENT)
							{
								eat(T_R_PARENT);
							}
							else
							{
								printSyntaxError(currentToken);
								errorFound = true;
							}
						}
						else
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_B)
	{
		eat(T_B);

		if (currentToken.getType() == T_ID)
		{
			eat(T_ID);
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_BLTZ)
	{
		eat(T_BLTZ);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_ID)
				{
					eat(T_ID);
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_NOP)
	{
		eat(T_NOP);
	}
	else if (currentToken.getType() == T_JAL)
	{
		eat(T_JAL);

		if (currentToken.getType() == T_ID)
		{
			eat(T_ID);
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_AND)
	{
		eat(T_AND);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);

					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);

						if (currentToken.getType() == T_R_ID)
						{
							eat(T_R_ID);
						}
						else
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else if (currentToken.getType() == T_ANDI)
	{
		eat(T_ANDI);

		if (currentToken.getType() == T_R_ID)
		{
			eat(T_R_ID);

			if (currentToken.getType() == T_COMMA)
			{
				eat(T_COMMA);

				if (currentToken.getType() == T_R_ID)
				{
					eat(T_R_ID);

					if (currentToken.getType() == T_COMMA)
					{
						eat(T_COMMA);

						if (currentToken.getType() == T_NUM)
						{
							eat(T_NUM);
						}
						else
						{
							printSyntaxError(currentToken);
							errorFound = true;
						}
					}
					else
					{
						printSyntaxError(currentToken);
						errorFound = true;
					}
				}
				else
				{
					printSyntaxError(currentToken);
					errorFound = true;
				}
			}
			else
			{
				printSyntaxError(currentToken);
				errorFound = true;
			}
		}
		else
		{
			printSyntaxError(currentToken);
			errorFound = true;
		}
	}
	else
	{
		printSyntaxError(currentToken);
		errorFound = true;
	}
}
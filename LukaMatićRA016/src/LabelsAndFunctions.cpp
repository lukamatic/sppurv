// Autor: Luka Matić Datum: 12.06.2020.

#include "LabelsAndFunctions.h"

using namespace std;

string Label::getName()
{
	return m_Name;
}

Instructions& Label::getInsturctions()
{
	return m_Insturctions;
}

string Function::getName()
{
	return m_Name;
}

Labels& Function::getLabels()
{
	return m_Labels;
}


void generateLabelsAndFunctions(Labels& labels, Functions& functions, LexicalAnalysis& lex)
{
	Function* currentFunc;

	for (auto it = lex.getTokenList().begin(); it != lex.getTokenList().end(); ++it)
		if (it->getType() == T_FUNC)
		{
			string funcName = (++it)->getValue();

			if (functionExists(functions, funcName))
			{
				cout << "\nFunction " + funcName + " has already been declared.\n" << endl;
				cout << endl << "Press enter to continue..." << endl;
				getchar();

				exit(EXIT_FAILURE);
			}


			Function* func = new Function(funcName);
			currentFunc = func;
			functions.push_back(func);

			Label* label = new Label(it->getValue());
			currentFunc->getLabels().push_back(label);
			labels.push_back(label);
		}
		else if (it->getType() == T_ID) 
			if ((++it)->getType() == T_COL)
			{
				string labelName = (--it)->getValue();

				if (labelExistsInFunction(currentFunc, labelName))
				{
					cout << endl << "\Label " + labelName + " has already been declared in function " + currentFunc->getName() + ".\n" << endl;
					cout << endl << "Press enter to continue..." << endl;
					getchar();

					exit(EXIT_FAILURE);
				}

				Label* label = new Label(labelName);
				currentFunc->getLabels().push_back(label);
				labels.push_back(label);
			}
}


bool functionExists(Functions& functions, string funcName)
{
	for (auto it = functions.begin(); it != functions.end(); ++it)
		if ((*it)->getName() == funcName)
			return true;

	return false;
}

bool labelExistsInFunction(Function* func, string labelName)
{
	for (auto it = func->getLabels().begin(); it != func->getLabels().end(); ++it)
		if ((*it)->getName() == labelName)
			return true;

	return false;
}


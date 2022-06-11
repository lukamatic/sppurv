// Autor: Luka Matić Datum: 12.06.2020.

#ifndef __LABELS_AND_FUNCTIONS__
#define __LABELS_AND_FUNCTIONS__

#include<list>

#include "IR.h"
#include "LexicalAnalysis.h"


class Label
{
public:
	Label(std::string labelName) : m_Name(labelName) {}

	std::string getName();

	Instructions& getInsturctions();

private:
	std::string m_Name;
	Instructions m_Insturctions;
};

typedef std::list<Label*> Labels;

class Function
{
public:
	Function(std::string funcName) : m_Name(funcName) {}

	std::string getName();

	Labels& getLabels();

private:
	std::string m_Name;
	Labels m_Labels;
};

typedef std::list<Function*> Functions;

void generateLabelsAndFunctions(Labels& labels, Functions& functions, LexicalAnalysis& lex);

bool functionExists(Functions& functions, std::string funcName);

bool labelExistsInFunction(Function* func, std::string labelName);

#endif // !__LABELS_AND_FUNCTIONS__


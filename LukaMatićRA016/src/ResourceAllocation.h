// Autor: Luka Matić Datum: 12.06.2020.

#ifndef __RESOURCE_ALLOCATION__
#define __RESOURCE_ALLOCATION__

#include <vector>

#include "IR.h"

enum InterferenceValue
{
	NO_INTERFERENCE,
	INTERFERENCE
};

typedef std::vector<std::vector<InterferenceValue>> InterferenceValues;

class InterferenceGraph
{
private:
	Variables* m_variables;
	InterferenceValues m_values;
	int m_size;

public:
	InterferenceGraph::InterferenceGraph(Variables* variables, int matrixSize);

	void build(Instructions instructions);

	void print();

	InterferenceValues& getValues();

	int getSize();

	void InterferenceGraph::simplify(std::stack<Variable*>& simplificationStack);
};

std::string regToString(Regs reg);

void doResourceAllocation(std::stack<Variable*>* simplificationStack, InterferenceGraph* interferenceGraph);



#endif // !__RESOURCE_ALLOCATION__

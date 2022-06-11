// Autor: Luka Matić Datum: 13.06.2020.

#include "ResourceAllocation.h"

using namespace std;

InterferenceGraph::InterferenceGraph(Variables* variables, int matrixSize) : m_variables(variables), m_size(matrixSize)
{
	m_values = InterferenceValues(m_size);

	for (int i = 0; i < m_size; i++)
		m_values[i] = vector<InterferenceValue>(m_size, NO_INTERFERENCE);
};

void InterferenceGraph::build(Instructions instructions)
{
	for (auto it1 = instructions.begin(); it1 != instructions.end(); ++it1)
		for (auto it2 = (*it1)->getDef().begin(); it2 != (*it1)->getDef().end(); ++it2)
			for (auto it3 = (*it1)->getOut().begin(); it3 != (*it1)->getOut().end(); ++it3)
				if((*it2)->getPosition() != (*it3)->getPosition())
				{
					if (m_values[(*it2)->getPosition()][(*it3)->getPosition()] != INTERFERENCE)
					{
						m_values[(*it2)->getPosition()][(*it3)->getPosition()] = INTERFERENCE;
						m_values[(*it3)->getPosition()][(*it2)->getPosition()] = INTERFERENCE;

						Variable* v1;
						Variable* v2;

						for (auto vit = m_variables->begin(); vit != m_variables->end(); ++vit)
							if ((*vit)->getPosition() == (*it2)->getPosition())
								v1 = *vit;
							else if ((*vit)->getPosition() == (*it3)->getPosition())
								v2 = *vit;

						v1->getNeighbors().push_back(v2);
						v2->getNeighbors().push_back(v1);
					}
				}
}

void InterferenceGraph::print()
{
	cout << "Interference Graph:" << endl << endl;

	for (int i = 0; i < m_size; i++)
	{
		for (int j = 0; j < m_size; j++)
			cout << m_values[i][j] << " ";

		cout << endl;
	}

	cout << endl;
}

InterferenceValues& InterferenceGraph::getValues()
{
	return m_values;
}

int InterferenceGraph::getSize()
{
	return m_size;
}


bool compare(Variable* v1, Variable* v2)
{
	return v1->getNeighbors().size() > v2->getNeighbors().size();
}

void InterferenceGraph::simplify(stack<Variable*>& simplificationStack)
{
	m_variables->sort(compare);

	for (auto it1 = m_variables->begin(); it1 != m_variables->end(); ++it1)
	{
		if ((*it1)->getNeighbors().size() < __REG_NUMBER__)
		{
			simplificationStack.push(*it1);

			for (auto it2 = m_variables->begin(); it2 != m_variables->end(); ++it2)
				(*it2)->getNeighbors().remove(*it1);

			m_variables->remove(*it1);

			simplify(simplificationStack);

			return;
		}
	}

	if (m_variables->size() > 0)
	{
		cout << endl << "Spill detected." << endl;
		cout << endl << "Press enter to continue..." << endl;
		getchar();

		exit(EXIT_FAILURE);
	}
}

string regToString(Regs reg)
{
	switch (reg)
	{
	case 1:
		return "$t0";
		break;
	case 2:
		return "$t1";
		break;
	case 3:
		return "$t2";
		break;
	case 4:
		return "$t3";
		break;
	default:
		return "$no_assign";
	}
}

void doResourceAllocation(stack<Variable*>* simplificationStack, InterferenceGraph* interferenceGraph)
{
	Variables checkedVariables;
	int regCounter = 1;

	while (!simplificationStack->empty())
	{
		Variable *current = simplificationStack->top();

		for (auto it = checkedVariables.begin(); it != checkedVariables.end(); ++it)
			if (interferenceGraph->getValues()[current->getPosition()][(*it)->getPosition()] == __INTERFERENCE__)
			{
				if (regCounter == __REG_NUMBER__)
				{
					cout << endl << "Spill detected." << endl;
					cout << endl << "Press enter to continue..." << endl;
					getchar();

					exit(EXIT_FAILURE);
				}
				else
					regCounter++;

				break;
			}

		current->setAssignment((Regs)regCounter);
		current->setName(regToString((Regs)regCounter));
		checkedVariables.push_back(current);
		simplificationStack->pop();
	}
}
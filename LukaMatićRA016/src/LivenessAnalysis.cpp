// Autor: Luka Matić Datum: 12.06.2020.

#include "LivenessAnalysis.h"

#include "IR.h"

using namespace std;

bool sortOrder(Variable* a, Variable* b)
{
	return a->getName() < b->getName(); 
}

bool checkEnd(vector<bool>& done)
{
	for (auto it = done.begin(); it != done.end(); ++it)
	{
		if (!*it)
			return false;
	}

	return true;
}

void livenessAnalysis(Instructions& instructions)
{
	for (auto it = instructions.rbegin(); it != instructions.rend(); ++it)
	{
		(*it)->getIn().clear();
		(*it)->getOut().clear();
	}

	Instructions::reverse_iterator it;

	Variables currIn;
	Variables currOut;
	Variables prevIn;
	Variables prevOut;

	bool end;
	vector<bool> done(instructions.size(), false);

	do
	{
		auto eit = done.begin();

		for (it = instructions.rbegin(); it != instructions.rend(); ++it, ++eit)
		{
			prevIn = (*it)->getIn();
			prevOut = (*it)->getOut();

			for (auto its = (*it)->getSucc().begin(); its != (*it)->getSucc().end(); ++its)
				for (auto iti = (*its)->getIn().begin(); iti != (*its)->getIn().end(); ++iti)
					(*it)->getOut().push_back(*iti);

			(*it)->getOut().sort(sortOrder);
			(*it)->getOut().unique();

			(*it)->getIn() = (*it)->getUse();

			for (auto ito = (*it)->getOut().begin(); ito != (*it)->getOut().end(); ++ito)
			{
				if (!variableExists((*ito)->getName(), (*it)->getDef()))
					(*it)->getIn().push_back(*ito);
			}

			(*it)->getIn().sort(sortOrder);
			(*it)->getIn().unique();

			currIn = (*it)->getIn();
			currOut = (*it)->getOut();

			if (prevIn == currIn && prevOut == currOut)
				*eit = true;
		}
	} while (!checkEnd(done));	
}

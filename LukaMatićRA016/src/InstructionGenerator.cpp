// Autor: Luka Matić Datum: 12.06.2020.

#include "InstructionGenerator.h"

using namespace std;

void generateUse(Variables& use, InstructionType type, Variable* src1, Variable* src2)
{
	if (type == I_ADD || type == I_SUB || type == I_AND || type == I_SW)
	{
		use.push_back(src1);
		use.push_back(src2);
	}
	else if (type == I_ADDI || type == I_LW || type == I_ANDI || type == I_BLTZ)
		use.push_back(src1);
}

void generateDef(Variables& def, InstructionType type, Variable* dst1)
{
	if (type == I_ADD || type == I_ADDI || type == I_SUB || type == I_LA || type == I_LW || type == I_LI || type == I_AND || type == I_ANDI)
		def.push_back(dst1);
}

void generateSucc(Instructions& instructions, Labels& labels)
{
	for (auto it = instructions.begin(); it != instructions.end(); ++it)
	{
		if ((*it)->getType() == I_BLTZ)
		{
			for (Label* label : labels)
				if ((*it)->getBranchingOrJumpingTo() == label->getName())
					(*it)->getSucc().push_back(label->getInsturctions().front());

			auto next = ++it;
			--it;

			if (next != instructions.end())
				(*it)->getSucc().push_back(*next);
		}
		else if ((*it)->getType() == I_B || (*it)->getType() == I_JAL)
		{
			for (Label* label : labels)
				if ((*it)->getBranchingOrJumpingTo() == label->getName())
					(*it)->getSucc().push_back(label->getInsturctions().front());
		}
		else
		{
			auto next = ++it;
			--it;

			if (next != instructions.end())
				(*it)->getSucc().push_back(*next);
		}
	}
}

void generatePred(Instructions& instructions)
{
	for (auto it1 = instructions.begin(); it1 != instructions.end(); ++it1)
	{
		auto next = ++it1;
		--it1;

		for (auto it2 = next; it2 != instructions.end(); ++it2)
			for (Instruction* inst : (*it1)->getSucc())
				if (*it2 == inst)
					(*it2)->getPred().push_back(*it1);
	}
}

void generateInstructions(Instructions& instructions, LexicalAnalysis& lex, Functions& functions, Labels& labels, Variables& regVariables, Variables& memVariables)
{
	Functions::iterator funcIt = functions.begin();
	Labels::iterator labelIt = (*funcIt)->getLabels().begin();
	int posCounter = 0;
	int labelCounter = 0;

	for (auto it = lex.getTokenList().begin(); it != lex.getTokenList().end(); ++it)
	{
		TokenType type = it->getType();

		if (type > 8 && type < 22) // checking if its an instuction
		{
			InstructionType instrType = (InstructionType)(type - 8); // converting TokenType into InstructionType
			Variables dst;
			Variables src;
			Variable* dst1 = NULL;
			Variable* src1 = NULL;
			Variable* src2 = NULL;
			bool branchingOrJumping = false;
			string branchingOrJumpingTo;
			string num = "";

			if (type == T_NOP)
				++it;
			else if (type == T_B)
			{
				if(!labelExistsInFunction(*funcIt, (++it)->getValue()))
				{
					cout << endl << "\Label " + (it)->getValue() + " hasn't been declared in function " + (*funcIt)->getName() + ".\n" << endl;
					cout << endl << "Press enter to continue..." << endl;
					getchar();

					exit(EXIT_FAILURE);
				}

				branchingOrJumpingTo = it->getValue();
			}
			else if (type == T_BLTZ)
			{
				++it;

				for (auto itv = regVariables.begin(); itv != regVariables.end(); ++itv)
					if ((*itv)->getName() == it->getValue())
						src1 = *itv;
				
				src.push_back(src1);
				++it;

				if (!labelExistsInFunction(*funcIt, (++it)->getValue()))
				{
					cout << endl << "\Label " + (it)->getValue() + " hasn't been declared in function " + (*funcIt)->getName() + ".\n" << endl;
					cout << endl << "Press enter to continue..." << endl;
					getchar();

					exit(EXIT_FAILURE);
				}

				branchingOrJumpingTo = it->getValue();
			}
			else if (type == T_JAL)
			{
				if(!functionExists(functions, (++it)->getValue()))
				{
					cout << "\nFunction " + it->getValue() + " hasn't been declared.\n" << endl;
					cout << endl << "Press enter to continue..." << endl;
					getchar();

					exit(EXIT_FAILURE);
				}

				branchingOrJumpingTo = it->getValue();
			}
			else if (type == T_SW)
			{
				++it;

				for (auto itv = regVariables.begin(); itv != regVariables.end(); ++itv)
					if ((*itv)->getName() == it->getValue())
						src1 = *itv;

				src.push_back(src1);
				++it;
				num = (++it)->getValue();
				++it;
				++it;

				for (auto itv = regVariables.begin(); itv != regVariables.end(); ++itv)
					if ((*itv)->getName() == it->getValue())
						src2 = *itv;

				src.push_back(src2);
			}
			else
			{
				++it;

				for (auto itv = regVariables.begin(); itv != regVariables.end(); ++itv)
					if ((*itv)->getName() == it->getValue())
						dst1 = *itv;

				dst.push_back(dst1);
				++it;
				type = (++it)->getType();

				if (type == T_R_ID)
				{
					for (auto itv = regVariables.begin(); itv != regVariables.end(); ++itv)
						if ((*itv)->getName() == it->getValue())
							src1 = *itv;

					src.push_back(src1);
					++it;
					type = (++it)->getType();

					if (type == T_R_ID)
					{
						for (auto itv = regVariables.begin(); itv != regVariables.end(); ++itv)
							if ((*itv)->getName() == it->getValue())
								src2 = *itv;

						src.push_back(src2);
					}
					else if (type == T_NUM)
						num = it->getValue();
				}
				else if (type == T_M_ID)
				{
					for (auto itv = memVariables.begin(); itv != memVariables.end(); ++itv)
						if ((*itv)->getName() == it->getValue())
							src1 = *itv;

					src.push_back(src1);
				}
				else if (type == T_NUM)
				{
					num = it->getValue();
					type = (++it)->getType();

					if (type == T_L_PARENT)
					{
						++it;

						for (auto itv = regVariables.begin(); itv != regVariables.end(); ++itv)
							if ((*itv)->getName() == it->getValue())
								src1 = *itv;

						src.push_back(src1);
					}
				}
			}

			Instruction *instruction = new Instruction(posCounter, instrType, dst, src, branchingOrJumpingTo, num);
			generateUse(instruction->getUse(), instrType, src1, src2);
			generateDef(instruction->getDef(), instrType, dst1);
			(*labelIt)->getInsturctions().push_back(instruction); 
			instructions.push_back(instruction);
			posCounter++;
		}
		else if (type == T_ID)
		{
			labelCounter++;

			if (labelCounter > 1)
				++labelIt;

			if (labelIt == (*funcIt)->getLabels().end())
			{
				++funcIt;

				if (funcIt != functions.end())
					labelIt = (*funcIt)->getLabels().begin();
			}
		}
	}

	generateSucc(instructions, labels);
	generatePred(instructions);
}
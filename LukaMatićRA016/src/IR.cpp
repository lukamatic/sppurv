// Autor: Luka Matić Datum: 12.06.2020.

#include "IR.h"

using namespace std;

void Variable::setType(VariableType type)
{
	m_type = type;
}

Variable::VariableType Variable::getType()
{
	return m_type;
}

string Variable::getName()
{
	return m_name;
}

int Variable::getPosition()
{
	return m_position;
}

list<Variable*>& Variable::getNeighbors()
{
	return m_neighbors;
}

int Variable::getValue()
{
	return m_value;
}

void Variable::setName(string name)
{
	m_name = name;
}

void Variable::setAssignment(Regs reg)
{
	m_assignment = reg;
}

string Instruction::toString()
{
	string ret = "\t";

	switch (m_type)
	{
	case 1:
		ret += "add\t\t";
		break;
	case 2:
		ret += "addi\t";
		break;
	case 3:
		ret += "sub\t\t";
		break;
	case 4:
		ret += "la\t\t";
		break;
	case 5:
		ret += "li\t\t";
		break;
	case 6:
		ret += "lw\t\t";
		break;
	case 7:
		ret += "sw\t\t";
		break;
	case 8:
		ret += "bltz\t" + (*m_src.begin())->getName() + ", " + m_branchingOrJumpingTo + "\n";
		return ret;
	case 9:
		ret += "b\t\t" + m_branchingOrJumpingTo + "\n";
		return ret;
	case 10:
		ret += "nop";
		break;
	case 11:
		ret += "addi\t$sp, $sp, -4\n";
		ret += "\tsw\t\t$ra, ($sp)\n";
		ret += "\tjal\t\t" + m_branchingOrJumpingTo + "\n";
		ret += "\tlw\t\t$ra, ($sp)\n";
		ret += "\taddiu\t$sp, $sp, 4\n";
		return ret;
	case 12:
		ret += "and\t\t";
		break;
	case 13:
		ret += "andi\t";
		break;
	default:
		ret = "";
	}

	if(!m_dst.empty())
		ret += (*m_dst.begin())->getName();
	
	if (m_num != "")
	{
		if (m_type == 2 || m_type == 13) // -> addi / andi
			ret += ", " + (*m_src.begin())->getName() + ", " + m_num;
		else if (m_type == 5) // -> li
			ret += ", " + m_num;
		else if(m_type == 6) // -> lw
			ret += ", " + m_num + "(" + (*m_src.begin())->getName() + ")";
		else // -> sw
		{
			auto it = m_src.begin();
			ret += (*it)->getName() + ", " + m_num + "(";
			ret += (*(++it))->getName() + ")";
		}
	}
	else if (!m_src.empty())
		for (auto it = m_src.begin(); it != m_src.end(); ++it)
			ret += ", " + (*it)->getName();

	ret += "\n";

	return ret;
}

bool variableExists(string variableName, Variables variables)
{
	for (auto it = variables.begin(); it != variables.end(); ++it)
		if ((*it)->getName() == variableName)
			return true;

	return false;
}

int getVariablePosition(string name, Variables variables)
{
	for (auto it = variables.begin(); it != variables.end(); ++it)
		if ((*it)->getName() == name)
			return (*it)->getPosition();

	cout << "\nProcess failed. Variable " + name + " hasn't been declared.\n";
	getchar();

	exit(EXIT_FAILURE);
}

Variable::VariableType getVariableType(string name, Variables variables)
{
	for (auto it = variables.begin(); it != variables.end(); ++it)
		if ((*it)->getName() == name)
			return (*it)->getType();

	cout << "\nProcess failed. Variable " + name + " hasn't been declared.\n";
	getchar();

	exit(EXIT_FAILURE);
}

void generateVariables(Variables& memVariables, Variables& regVariables, LexicalAnalysis& lex)
{
	// nisam sig da li pos krece od 0 ili od 1
	int memPosCounter = 0;
	int regPosCounter = 0;

	for (auto it = lex.getTokenList().begin(); it != lex.getTokenList().end(); ++it)
		if (it->getType() == T_MEM)
		{
			string name = (++it)->getValue();
			int value = stoi((++it)->getValue());
			Variable *var = new Variable(name, memPosCounter++, Variable::MEM_VAR, value);

			if (variableExists(name, memVariables))
			{
				cout << "\nProcess failed. Variable " + name + " has already been declared.\n";
				getchar();

				exit(EXIT_FAILURE);
			}

			memVariables.push_back(var);
		}
		else if (it->getType() == T_REG)
		{
			string name = (++it)->getValue();
			Variable *var = new Variable(name, regPosCounter++);
			var->setType(Variable::REG_VAR);

			if (variableExists(name, regVariables))
			{
				cout << "\nProcess failed. Variable " + name + " has already been declared.\n";
				getchar();

				exit(EXIT_FAILURE);
			}

			regVariables.push_back(var);
		}
}

InstructionType Instruction::getType()
{
	return m_type;
}

Variables& Instruction::getUse()
{
	return m_use;
}

Variables& Instruction::getDef()
{
	return m_def;
}

Variables& Instruction::getIn()
{
	return m_in;
}

Variables& Instruction::getOut()
{
	return m_out;
}

list<Instruction*>& Instruction::getSucc()
{
	return m_succ;
}

list<Instruction*>& Instruction::getPred()
{
	return m_pred;
}

string Instruction::getBranchingOrJumpingTo()
{
	return m_branchingOrJumpingTo;
}
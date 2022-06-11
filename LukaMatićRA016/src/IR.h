// Autor: Luka Matić Datum: 12.06.2020.

#ifndef __IR__
#define __IR__

#include "Types.h"
#include "LexicalAnalysis.h"

/**
 * This class represents one variable from program code.
 */
class Variable
{
public:
	enum VariableType
	{
		MEM_VAR,
		REG_VAR,
		NO_TYPE
	};

	Variable() : m_type(NO_TYPE), m_name(""), m_position(-1), m_assignment(no_assign) {}
	Variable(std::string name, int pos) : m_type(NO_TYPE), m_name(name), m_position(pos), m_assignment(no_assign) {}
	Variable(std::string name, int pos, VariableType type) : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign) {}
	Variable(std::string name, int pos, VariableType type, int value) : m_type(type), m_name(name), m_position(pos), m_assignment(no_assign), m_value(value) {}

	void setType(VariableType type);

	VariableType getType();

	std::string getName();

	int getPosition();

	std::list<Variable*>& getNeighbors();

	int getValue();

	void setName(std::string);

	void setAssignment(Regs reg);

private:
	VariableType m_type;
	std::string m_name;
	int m_position;
	Regs m_assignment;
	std::list<Variable*> m_neighbors;
	int m_value;
};

/**
* This type represents list of variables from program code.
*/
typedef std::list<Variable*> Variables;


bool variableExists(std::string variableName, Variables variables);

int getVariablePosition(std::string name, Variables variables);

Variable::VariableType getVariableType(std::string name, Variables variables);

void generateVariables(Variables& memVariables, Variables& regVariables, LexicalAnalysis& lex);


/**
 * This class represents one instruction in program code.
 */
class Instruction
{
public:
	Instruction () : m_position(0), m_type(I_NO_TYPE) {}
	Instruction (int pos, InstructionType type, Variables& dst, Variables& src, std::string branchingOrJumpingTo, std::string num) :
		m_position(pos), m_type(type), m_dst(dst), m_src(src), m_branchingOrJumpingTo(branchingOrJumpingTo), m_num(num) {}

	InstructionType getType();

	Variables& getUse();

	Variables& getDef();

	Variables& getIn();

	Variables& getOut();

	std::list<Instruction*>& getSucc();

	std::list<Instruction*>& getPred();

	std::string getBranchingOrJumpingTo();

	std::string toString();

private:
	int m_position;
	InstructionType m_type;
	
	Variables m_dst;
	Variables m_src;

	Variables m_use;
	Variables m_def;
	Variables m_in;
	Variables m_out;
	std::list<Instruction*> m_succ;
	std::list<Instruction*> m_pred;

	std::string m_branchingOrJumpingTo; // Name of the label that instruction branches or jumps to
	std::string m_num;
};


/**
 * This type represents list of instructions from program code.
 */
typedef std::list<Instruction*> Instructions;

#endif

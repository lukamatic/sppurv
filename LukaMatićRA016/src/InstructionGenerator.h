// Autor: Luka Matić Datum: 12.06.2020.

#ifndef __INSTRUCTION_GENERATOR__
#define __INSTRUCTION_GENERATOR__

#include "IR.h"
#include "LabelsAndFunctions.h"

void generateUse(Variables& use, InstructionType type, Variable* src1, Variable* src2);

void generateDef(Variables& use, InstructionType type, Variable* dst1);

void generateSucc(Instructions& instructions, Labels& labels);

void generatePred(Instructions& instructions);

void generateInstructions(Instructions& instructions, LexicalAnalysis& lex, Functions& functions, Labels& labels, Variables& regVariables, Variables& memVariables);

#endif // !__INSTRUCTION_GENERATOR__

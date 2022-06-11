// Autor: Luka Matić Datum: 14.06.2020.

#include <iostream>
#include <exception>

#include "LexicalAnalysis.h"
#include "SyntaxAnalysis.h"
#include "IR.h"
#include "LabelsAndFunctions.h"
#include "InstructionGenerator.h"
#include "LivenessAnalysis.h"
#include "ResourceAllocation.h"

using namespace std;

int main()
{
	try
	{
		// Lexical Analyisis

		std::string fileName = ".\\..\\examples\\simple.mavn";
		bool retVal = false;

		LexicalAnalysis lex;

		if (!lex.readInputFile(fileName))
			throw runtime_error("\nException! Failed to open input file!\n");

		lex.initialize();

		retVal = lex.Do();

		if (retVal)
		{
			cout << "Lexical analysis finished successfully!" << endl << endl;
			lex.printTokens();
		}
		else
		{
			lex.printLexError();
			throw runtime_error("\nException! Lexical analysis failed!\n");
		}

		// Syntax Analysis

		SyntaxAnalysis syntax(lex);

		if (syntax.Do())
			cout << endl << "Syntax analysis finished successfully!" << endl << endl;
		else
		{
			cout << endl << "Syntax analysis failed!" << endl << endl;
			cout << endl << "Press enter to continue..." << endl;
			getchar();

			exit(EXIT_FAILURE);
		}

		// Generating labels and functions

		Labels labels;
		Functions functions;
		generateLabelsAndFunctions(labels, functions, lex);

		// Generating lists of memory variables and register variables

		Variables memVariables;
		Variables regVariables;
		generateVariables(memVariables, regVariables, lex);
		
		// Generating list of insturctions

		Instructions instructions;
		generateInstructions(instructions, lex, functions, labels, regVariables, memVariables);

		// Liveness Analysis

		livenessAnalysis(instructions);

		// Resource Allocation

		InterferenceGraph interferenceGraph(&regVariables, instructions.size());
		interferenceGraph.build(instructions);
		interferenceGraph.print();

		stack<Variable*> simplificationStack;
		interferenceGraph.simplify(simplificationStack);

		doResourceAllocation(&simplificationStack, &interferenceGraph);
		
		cout << endl << "Resource allocation finished successfully!" << endl << endl;
			
		// Writing code to output file

		fileName.pop_back(); // n
		fileName.pop_back(); // v
		fileName.pop_back(); // a
		fileName.pop_back(); // m
		fileName += "s";

		ofstream outputFile(fileName);
			
		if (outputFile.is_open())
		{
			// .globl
				
			for (auto it = functions.begin(); it != functions.end(); ++it)
				outputFile << ".globl " + (*it)->getName() + "\n";

			// .data
				
			outputFile << "\n.data\n";

			for (auto it = memVariables.begin(); it != memVariables.end(); ++it)
				outputFile << (*it)->getName() + ":\t\t.word " << (*it)->getValue() << "\n";

			// .text

			outputFile << "\n.text\n";

			for (auto itf = functions.begin(); itf != functions.end(); ++itf)
			{
				for (auto itl = (*itf)->getLabels().begin(); itl != (*itf)->getLabels().end(); ++itl)
				{
					outputFile << (*itl)->getName() + ":\n";

					for (auto iti = (*itl)->getInsturctions().begin(); iti != (*itl)->getInsturctions().end(); ++iti)
						outputFile << (*iti)->toString();
				}

				outputFile << "\tjr\t\t$ra\n";
			}
					
			outputFile.close();
		}
		else
		{
			cout << endl << "Unable to open file";
			cout << endl << "Press enter to continue..." << endl;
			getchar();

			exit(EXIT_FAILURE);
		}
	}
	catch (runtime_error e)
	{
		cout << e.what() << endl;
		getchar();

		return 1;
	}

	cout << endl << "Process completed." << endl << endl;

	return 0;
}

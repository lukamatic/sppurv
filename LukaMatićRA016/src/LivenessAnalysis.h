// Autor: Luka Matić Datum: 12.06.2020.

#ifndef __LIVENESS_ANALYSIS__
#define __LIVENESS_ANALYSIS__

#include "IR.h"

bool checkEnd(std::vector<bool>& done);

void livenessAnalysis(Instructions& instructions);

#endif
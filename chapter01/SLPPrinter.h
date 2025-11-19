#pragma once
#include "SLP.h"
#include <iosfwd>

namespace slp {


void PrintSource(const A_Stm& program, std::ostream& os);

void PrintStmt(const AStmt& stm, std::ostream& os);
void PrintExp(const AExp& exp, std::ostream& os);

} // namespace slp

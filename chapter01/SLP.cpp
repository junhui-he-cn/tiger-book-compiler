#include "SLP.h"

namespace slp {
A_Stm A_CompoundStm(A_Stm Stm1, A_Stm Stm2) {
  return std::make_unique<CompoundStm>(std::move(Stm1), std::move(Stm2));
}

A_Stm A_AssignStm(std::string Id, A_Exp Exp) {
  return std::make_unique<AssignStm>(std::move(Id), std::move(Exp));
}

A_Stm A_PrintStm(A_ExpList Exps) {
  return std::make_unique<PrintStm>(std::move(Exps));
}

A_Exp A_IdExp(std::string Id) { return std::make_unique<IdExp>(std::move(Id)); }

A_Exp A_NumExp(int Num) { return std::make_unique<NumExp>(Num); }

A_Exp A_OpExp(A_Exp Left, ABinOp Oper, A_Exp Right) {
  return std::make_unique<OpExp>(std::move(Left), Oper, std::move(Right));
}

A_Exp A_EseqExp(A_Stm Stm, A_Exp Exp) {
  return std::make_unique<EseqExp>(std::move(Stm), std::move(Exp));
}

A_ExpList A_PairExpList(A_Exp Head, A_ExpList Tail) {
  return std::make_unique<PairExpList>(std::move(Head), std::move(Tail));
}

A_ExpList A_LastExpList(A_Exp Last) {
  return std::make_unique<LastExpList>(std::move(Last));
}
} // namespace slp

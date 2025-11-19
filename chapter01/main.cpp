#include <iostream>
#include <fstream>
#include <iterator>

#include "SLP.h"
#include "SLPPrinter.h"
#include "SLPGraphviz.h"

using namespace slp;
A_Stm prog(void) {
  return A_CompoundStm(
      A_AssignStm("a", A_OpExp(A_NumExp(5), ABinOp::Plus, A_NumExp(3))),
      A_CompoundStm(
          A_AssignStm("b",
                      A_EseqExp(A_PrintStm(A_PairExpList(
                                    A_IdExp("a"),
                                    A_LastExpList(A_OpExp(A_IdExp("a"), ABinOp::Minus,
                                                          A_NumExp(1))))),
                                A_OpExp(A_NumExp(10), ABinOp::Times, A_IdExp("a")))),
          A_PrintStm(A_LastExpList(A_IdExp("b")))));
}

int MaxArg(const AStmt& Stm);

int MaxArg(const AExp& Exp) {
  switch (Exp.Kind) {
  case AExp::ExpKind::Id:
  case AExp::ExpKind::Num:
    return 1;
  case AExp::ExpKind::Op:{
    const auto &OpE = static_cast<const OpExp &>(Exp);
    int NL = MaxArg(*OpE.Left);
    int NR = MaxArg(*OpE.Left);
    return std::max(NL, NR);
  }
  case AExp::ExpKind::Eseq: {
    const auto &Eseq = static_cast<const EseqExp &>(Exp);
    int N = MaxArg(*Eseq.Stm);
    return N;
  }
  }
}

int MaxArg(const AExpList& ExpList) {
  switch (ExpList.Kind) {
  case AExpList::ExpListKind::Pair: {
    const auto &List = static_cast<const PairExpList &>(ExpList);
    int NHead = MaxArg(*List.Head);
    int NTail = MaxArg(*List.Tail);
    return NHead + NTail;
  }
  case AExpList::ExpListKind::Last: {
    const auto &List = static_cast<const LastExpList &>(ExpList);
    int N = MaxArg(*List.Last);
    return N;
  }
    break;
  }
}

int MaxArg(const AStmt& Stm) {
  switch (Stm.Kind) {
  case AStmt::StmtKind::Compound: {
    const auto &ComStm = static_cast<const CompoundStm &>(Stm);
    int N1 = MaxArg(*ComStm.Stm1);
    int N2 = MaxArg(*ComStm.Stm2);
    return std::max(N1, N2);
  }
  case AStmt::StmtKind::Assign:{
    const auto &AStm = static_cast<const AssignStm &>(Stm);
    int n = MaxArg(*AStm.Exp);
    return n;
  }
  case AStmt::StmtKind::Print: {
    const auto &PStm = static_cast<const PrintStm &>(Stm);
    int n = MaxArg(*PStm.Exps);
    return n;
  }
  }
}

int main() {

  auto p = prog();

  PrintSource(p, std::cout);

 std::cout << MaxArg(*p) << std::endl;


  std::ofstream fout("ast.dot");
  fout << ToGraphviz(p);

  std::cout << "Hello, World!" << std::endl;
  return 0;
}
#ifndef SLP_H
#define SLP_H

#include <memory>
#include <string>

namespace slp {
enum class ABinOp { Plus, Minus, Times, Div };

struct AStmt;
struct AExp;
struct AExpList;

struct AStmt {
  enum class StmtKind { Compound, Assign, Print };
  StmtKind Kind;
  explicit AStmt(StmtKind K) : Kind(K) {}
  virtual ~AStmt() = default;
};

struct CompoundStm final : public AStmt {
  std::unique_ptr<AStmt> Stm1;
  std::unique_ptr<AStmt> Stm2;
  CompoundStm(std::unique_ptr<AStmt> S1, std::unique_ptr<AStmt> S2)
      : AStmt(StmtKind::Compound), Stm1(std::move(S1)), Stm2(std::move(S2)) {}
};

struct AssignStm final : public AStmt {
  std::string Id;
  std::unique_ptr<AExp> Exp;
  AssignStm(std::string Id, std::unique_ptr<AExp> E)
      : AStmt(StmtKind::Assign), Id(std::move(Id)), Exp(std::move(E)) {}
};

struct PrintStm final : public AStmt {
  std::unique_ptr<AExpList> Exps;
  explicit PrintStm(std::unique_ptr<AExpList> Exps)
      : AStmt(StmtKind::Print), Exps(std::move(Exps)) {}
};

struct AExp {
  enum class ExpKind { Id, Num, Op, Eseq };
  ExpKind Kind;
  explicit AExp(ExpKind K) : Kind(K) {}
  virtual ~AExp() = default;
};

struct IdExp final : public AExp {
  std::string Id;
  explicit IdExp(std::string Id) : AExp(ExpKind::Id), Id(std::move(Id)) {}
};

struct NumExp final : public AExp {
  int Value;
  explicit NumExp(int V) : AExp(ExpKind::Num), Value(V) {}
};

struct OpExp final : public AExp {
  std::unique_ptr<AExp> Left;
  ABinOp Oper;
  std::unique_ptr<AExp> Right;
  OpExp(std::unique_ptr<AExp> L, ABinOp Op, std::unique_ptr<AExp> R)
      : AExp(ExpKind::Op), Left(std::move(L)), Oper(Op), Right(std::move(R)) {}
};

struct EseqExp final : public AExp {
  std::unique_ptr<AStmt> Stm;
  std::unique_ptr<AExp> Exp;
  EseqExp(std::unique_ptr<AStmt> S, std::unique_ptr<AExp> E)
      : AExp(ExpKind::Eseq), Stm(std::move(S)), Exp(std::move(E)) {}
};

// 表达式列表
struct AExpList {
  enum class ExpListKind { Pair, Last };
  ExpListKind Kind;
  explicit AExpList(ExpListKind K) : Kind(K) {}
  virtual ~AExpList() = default;
};

struct PairExpList final : public AExpList {
  std::unique_ptr<AExp> Head;
  std::unique_ptr<AExpList> Tail;
  PairExpList(std::unique_ptr<AExp> H, std::unique_ptr<AExpList> T)
      : AExpList(ExpListKind::Pair), Head(std::move(H)), Tail(std::move(T)) {}
};

struct LastExpList final : public AExpList {
  std::unique_ptr<AExp> Last;
  explicit LastExpList(std::unique_ptr<AExp> L)
      : AExpList(ExpListKind::Last), Last(std::move(L)) {}
};

using A_Stm = std::unique_ptr<AStmt>;
using A_Exp = std::unique_ptr<AExp>;
using A_ExpList = std::unique_ptr<AExpList>;

A_Stm A_CompoundStm(A_Stm Stm1, A_Stm Stm2);
A_Stm A_AssignStm(std::string Id, A_Exp Exp);
A_Stm A_PrintStm(A_ExpList Exps);

A_Exp A_IdExp(std::string Id);
A_Exp A_NumExp(int Num);
A_Exp A_OpExp(A_Exp Left, ABinOp Oper, A_Exp Right);
A_Exp A_EseqExp(A_Stm Stm, A_Exp Exp);

A_ExpList A_PairExpList(A_Exp Head, A_ExpList Tail);
A_ExpList A_LastExpList(A_Exp Last);

} // namespace slp

#endif // SLP_H

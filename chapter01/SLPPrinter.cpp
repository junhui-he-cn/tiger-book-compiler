#include "SLPPrinter.h"
#include <climits>
#include <iostream>
#include <string>

namespace slp {

static void PrintStmtImpl(const AStmt &stm, std::ostream &os, int indent);
static void PrintExpImpl(const AExp &exp, std::ostream &os, int parentPrec);
static void PrintListImpl(const AExpList &list, std::ostream &os,
                          int parentPrec);
static int Prec(ABinOp op);
static void Indent(std::ostream &os, int indent);

//===----------------------------------------------------------------------===//
// Public APIs
//===----------------------------------------------------------------------===//

void PrintSource(const A_Stm &program, std::ostream &os) {
  if (!program) {
    os << "<empty>\n";
    return;
  }
  PrintStmtImpl(*program, os, /*indent=*/0);
  os << '\n';
}

void PrintStmt(const AStmt &stm, std::ostream &os) {
  PrintStmtImpl(stm, os, /*indent=*/0);
}

void PrintExp(const AExp &exp, std::ostream &os) {
  PrintExpImpl(exp, os, /*parentPrec=*/INT_MAX);
}

//===----------------------------------------------------------------------===//
// Helpers
//===----------------------------------------------------------------------===//

static void Indent(std::ostream &os, int indent) {
  for (int i = 0; i < indent; ++i)
    os << ' ';
}

static int Prec(ABinOp op) {
  switch (op) {
  case ABinOp::Times:
    return 2;
  case ABinOp::Div:
    return 2;
  case ABinOp::Plus:
    return 1;
  case ABinOp::Minus:
    return 1;
  }
  return 0;
}

static const char *OpText(ABinOp op) {
  switch (op) {
  case ABinOp::Plus:
    return "+";
  case ABinOp::Minus:
    return "-";
  case ABinOp::Times:
    return "*";
  case ABinOp::Div:
    return "/";
  }
  return "?";
}

// Print 语句
static void PrintStmtImpl(const AStmt &stm, std::ostream &os, int indent) {
  switch (stm.Kind) {
  case AStmt::StmtKind::Compound: {
    const auto &n = static_cast<const CompoundStm &>(stm);
    Indent(os, indent);
    os << "{\n";
    if (n.Stm1)
      PrintStmtImpl(*n.Stm1, os, indent + 2);
    else {
      Indent(os, indent + 2);
      os << "<null-stmt>";
    }
    os << ";\n";
    if (n.Stm2)
      PrintStmtImpl(*n.Stm2, os, indent + 2);
    else {
      Indent(os, indent + 2);
      os << "<null-stmt>";
    }
    os << '\n';
    Indent(os, indent);
    os << "}";
    break;
  }
  case AStmt::StmtKind::Assign: {
    const auto &n = static_cast<const AssignStm &>(stm);
    Indent(os, indent);
    os << n.Id << " := ";
    if (n.Exp)
      PrintExpImpl(*n.Exp, os, INT_MAX);
    else
      os << "<null-exp>";
    break;
  }
  case AStmt::StmtKind::Print: {
    const auto &n = static_cast<const PrintStm &>(stm);
    Indent(os, indent);
    os << "print(";
    if (n.Exps)
      PrintListImpl(*n.Exps, os, INT_MAX);
    os << ")";
    break;
  }
  }
}

static void PrintExpImpl(const AExp &exp, std::ostream &os, int parentPrec) {
  switch (exp.Kind) {
  case AExp::ExpKind::Id: {
    const auto &n = static_cast<const IdExp &>(exp);
    os << n.Id;
    break;
  }
  case AExp::ExpKind::Num: {
    const auto &n = static_cast<const NumExp &>(exp);
    os << n.Value;
    break;
  }
  case AExp::ExpKind::Op: {
    const auto &n = static_cast<const OpExp &>(exp);
    int myPrec = Prec(n.Oper);
    bool needParens = myPrec < parentPrec;
    if (needParens)
      os << "(";
    if (n.Left)
      PrintExpImpl(*n.Left, os, myPrec);
    else
      os << "<null>";
    os << ' ' << OpText(n.Oper) << ' ';
    if (n.Right)
      PrintExpImpl(*n.Right, os, myPrec - 1 /*右结合防多括号*/);
    else
      os << "<null>";
    if (needParens)
      os << ")";
    break;
  }
  case AExp::ExpKind::Eseq: {
    const auto &n = static_cast<const EseqExp &>(exp);
    os << "eseq ";
    os << "{ ";
    if (n.Stm) {
      PrintStmtImpl(*n.Stm, os, 0);
    } else
      os << "<null-stmt>";
    os << " ; ";
    if (n.Exp) {
      PrintExpImpl(*n.Exp, os, INT_MAX);
    } else
      os << "<null-exp>";
    os << " }";
    break;
  }
  }
}

static void PrintListImpl(const AExpList &list, std::ostream &os,
                          int parentPrec) {
  switch (list.Kind) {
  case AExpList::ExpListKind::Last: {
    const auto &n = static_cast<const LastExpList &>(list);
    if (n.Last)
      PrintExpImpl(*n.Last, os, parentPrec);
    else
      os << "<null-exp>";
    break;
  }
  case AExpList::ExpListKind::Pair: {
    const auto &n = static_cast<const PairExpList &>(list);
    if (n.Head)
      PrintExpImpl(*n.Head, os, parentPrec);
    else
      os << "<null-exp>";
    os << ", ";
    if (n.Tail)
      PrintListImpl(*n.Tail, os, parentPrec);
    else
      os << "<null-list>";
    break;
  }
  }
}

} // namespace slp

#include "SLPGraphviz.h"
#include <sstream>

namespace slp {

namespace {

int NextId() {
  static int id = 0;
  return id++;
}

std::string NodeLabel(const AStmt& stm) {
  switch (stm.Kind) {
  case AStmt::StmtKind::Compound: return "Compound";
  case AStmt::StmtKind::Assign:   return "Assign";
  case AStmt::StmtKind::Print:    return "Print";
  }
  return "Stmt?";
}

std::string NodeLabel(const AExp& exp) {
  switch (exp.Kind) {
  case AExp::ExpKind::Id:   return "Id";
  case AExp::ExpKind::Num:  return "Num";
  case AExp::ExpKind::Op:   return "Op";
  case AExp::ExpKind::Eseq: return "Eseq";
  }
  return "Exp?";
}

std::string NodeLabel(const AExpList& list) {
  switch (list.Kind) {
  case AExpList::ExpListKind::Pair: return "Pair";
  case AExpList::ExpListKind::Last: return "Last";
  }
  return "List?";
}

// 递归生成节点与边
int EmitStmt(std::ostringstream& out, const AStmt& stm);
int EmitExp(std::ostringstream& out, const AExp& exp);
int EmitList(std::ostringstream& out, const AExpList& list);

int EmitStmt(std::ostringstream& out, const AStmt& stm) {
  int id = NextId();
  out << "  n" << id << " [label=\"" << NodeLabel(stm) << "\"];\n";
  switch (stm.Kind) {
  case AStmt::StmtKind::Compound: {
    const auto& n = static_cast<const CompoundStm&>(stm);
    if (n.Stm1) {
      int c = EmitStmt(out, *n.Stm1);
      out << "  n" << id << " -> n" << c << " [label=\"stm1\"];\n";
    }
    if (n.Stm2) {
      int c = EmitStmt(out, *n.Stm2);
      out << "  n" << id << " -> n" << c << " [label=\"stm2\"];\n";
    }
    break;
  }
  case AStmt::StmtKind::Assign: {
    const auto& n = static_cast<const AssignStm&>(stm);
    out << "  n" << id << " [label=\"Assign: " << n.Id << "\"];\n";
    if (n.Exp) {
      int c = EmitExp(out, *n.Exp);
      out << "  n" << id << " -> n" << c << ";\n";
    }
    break;
  }
  case AStmt::StmtKind::Print: {
    const auto& n = static_cast<const PrintStm&>(stm);
    if (n.Exps) {
      int c = EmitList(out, *n.Exps);
      out << "  n" << id << " -> n" << c << ";\n";
    }
    break;
  }
  }
  return id;
}

int EmitExp(std::ostringstream& out, const AExp& exp) {
  int id = NextId();
  out << "  n" << id << " [label=\"" << NodeLabel(exp) << "\"];\n";
  switch (exp.Kind) {
  case AExp::ExpKind::Id: {
    const auto& n = static_cast<const IdExp&>(exp);
    out << "  n" << id << " [label=\"Id: " << n.Id << "\"];\n";
    break;
  }
  case AExp::ExpKind::Num: {
    const auto& n = static_cast<const NumExp&>(exp);
    out << "  n" << id << " [label=\"Num: " << n.Value << "\"];\n";
    break;
  }
  case AExp::ExpKind::Op: {
    const auto& n = static_cast<const OpExp&>(exp);
    out << "  n" << id << " [label=\"Op: ";
    switch (n.Oper) {
    case ABinOp::Plus: out << "+"; break;
    case ABinOp::Minus: out << "-"; break;
    case ABinOp::Times: out << "*"; break;
    case ABinOp::Div: out << "/"; break;
    }
    out << "\"];\n";
    if (n.Left) {
      int l = EmitExp(out, *n.Left);
      out << "  n" << id << " -> n" << l << " [label=\"left\"];\n";
    }
    if (n.Right) {
      int r = EmitExp(out, *n.Right);
      out << "  n" << id << " -> n" << r << " [label=\"right\"];\n";
    }
    break;
  }
  case AExp::ExpKind::Eseq: {
    const auto& n = static_cast<const EseqExp&>(exp);
    if (n.Stm) {
      int s = EmitStmt(out, *n.Stm);
      out << "  n" << id << " -> n" << s << " [label=\"stm\"];\n";
    }
    if (n.Exp) {
      int e = EmitExp(out, *n.Exp);
      out << "  n" << id << " -> n" << e << " [label=\"exp\"];\n";
    }
    break;
  }
  }
  return id;
}

int EmitList(std::ostringstream& out, const AExpList& list) {
  int id = NextId();
  out << "  n" << id << " [label=\"" << NodeLabel(list) << "\"];\n";
  switch (list.Kind) {
  case AExpList::ExpListKind::Pair: {
    const auto& n = static_cast<const PairExpList&>(list);
    if (n.Head) {
      int h = EmitExp(out, *n.Head);
      out << "  n" << id << " -> n" << h << " [label=\"head\"];\n";
    }
    if (n.Tail) {
      int t = EmitList(out, *n.Tail);
      out << "  n" << id << " -> n" << t << " [label=\"tail\"];\n";
    }
    break;
  }
  case AExpList::ExpListKind::Last: {
    const auto& n = static_cast<const LastExpList&>(list);
    if (n.Last) {
      int c = EmitExp(out, *n.Last);
      out << "  n" << id << " -> n" << c << " [label=\"last\"];\n";
    }
    break;
  }
  }
  return id;
}

} // namespace

std::string ToGraphviz(const A_Stm& program) {
  std::ostringstream out;
  out << "digraph AST {\n";
  if (program) EmitStmt(out, *program);
  out << "}\n";
  return out.str();
}

} // namespace slp

#pragma once
#include <iostream>
#include "SLP.h"  // 你的 AST 头文件

namespace slp {

inline const char* binOpToStr(ABinOp op) {
  switch (op) {
    case ABinOp::Plus:  return "+";
    case ABinOp::Minus: return "-";
    case ABinOp::Times: return "*";
    case ABinOp::Div:   return "/";
  }
  return "?";
}

void printStm(const AStmt* s, std::ostream& os, int indent = 0);
void printExp(const AExp* e, std::ostream& os);
void printExpList(const AExpList* l, std::ostream& os);

} // namespace slp

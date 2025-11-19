#pragma once
#include "SLP.h"
#include <string>

namespace slp {

// 将 AST 输出为 Graphviz DOT 格式字符串
std::string ToGraphviz(const A_Stm& program);

} // namespace slp

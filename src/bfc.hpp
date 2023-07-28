#pragma once

// change file name to bfc.hpp and bf.hpp is changed to compiler.hpp later

#include <string>
#include <vector>
#include <memory>

enum TokenKind {
  TK_CMD,
};

struct Token {
  TokenKind kind;
  char value;
};

enum NodeKind {
  ND_INC_PTR,
  ND_DEC_PTR,
  ND_INC_VAL,
  ND_DEC_VAL,
  ND_PUT_CHAR,
  ND_GET_CHAR,
  ND_LOOP_START,
  ND_LOOP_END,
};

struct Node {
  NodeKind kind;
  std::vector<std::shared_ptr<Node>> children;  // for LOOP_START
};

const std::string kCorrectTokens = R"(<>+-.,[])";

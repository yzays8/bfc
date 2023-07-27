#pragma once

#include <vector>
#include <list>
#include <memory>
#include <string>
#include <fstream>

enum TokenKind {
  TK_CMD,
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

struct Token {
  TokenKind kind;
  char value;
};

struct Node {
  NodeKind kind;
  std::vector<std::shared_ptr<Node>> children;  // for LOOP_START
};

const std::string kCorrectTokens = R"(<>+-.,[])";
constexpr int kMemSize = 30000;

class Compiler {
 public:
  Compiler();
  void GetCodeC(const std::string path);

 private:
  std::ifstream Load(const std::string path);
  void Lex(const std::string path);
  void Parse();

  void Program();
  std::shared_ptr<Node> Command();

  void GenerateC(std::shared_ptr<Node> node, int depth);
  void PrintC();

  std::list<std::shared_ptr<Token>> tokens_;
  std::list<std::shared_ptr<Token>>::iterator token_iter_;
  std::list<std::shared_ptr<Node>> code_;
  int depth_;
};

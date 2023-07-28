#pragma once

#include <memory>
#include <list>

#include "bfc.hpp"

class Parser {
 public:
  Parser();
  std::list<std::shared_ptr<Node>>& Parse(std::list<std::shared_ptr<Token>>& tokens);

 private:
  void Program();
  std::shared_ptr<Node> Command();

  std::list<std::shared_ptr<Token>> tokens_;
  std::list<std::shared_ptr<Token>>::iterator token_iter_;
  std::list<std::shared_ptr<Node>> code_;
  int depth_;
};

#pragma once

#include <memory>
#include <list>

#include "bfc.hpp"

class Parser {
 public:
  Parser();
  std::list<std::unique_ptr<Node>>& Parse(const std::list<std::unique_ptr<Token>>& tokens);

 private:
  void Initialize(const std::list<std::unique_ptr<Token>>& tokens);

  void Program();
  std::unique_ptr<Node> Command();

  std::list<std::unique_ptr<Token>>::const_iterator token_iter_;
  std::list<std::unique_ptr<Token>>::const_iterator token_end_;
  std::list<std::unique_ptr<Node>> nodes_;
  int depth_;
};

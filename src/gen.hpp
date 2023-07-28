#pragma once

#include <memory>
#include <list>

#include "bfc.hpp"

constexpr int kMemSize = 30000;

class Generator {
 public:
  Generator();
  void PrintC(std::list<std::shared_ptr<Node>>& code);

 private:
  void GenerateC(std::shared_ptr<Node> node, int depth);

  std::list<std::shared_ptr<Node>> code_;
};

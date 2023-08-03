#pragma once

#include <memory>
#include <list>

#include "bfc.hpp"

constexpr int kMemSize = 30000;

class Generator {
 public:
  Generator();
  void PrintC(const std::list<std::unique_ptr<Node>>& code);

 private:
  void GenerateC(const std::unique_ptr<Node>& node, int depth);
};

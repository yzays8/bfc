#include <iostream>
#include <memory>

#include "lexer.hpp"

Lexer::Lexer() : tokens_{} {}

std::list<std::shared_ptr<Token>>& Lexer::Lex(std::ifstream ifs) {
  char data;
  while (ifs.get(data)) {
    // ignore all non-token characters
    if (kCorrectTokens.find(data) != std::string::npos) {
      tokens_.push_back(std::make_shared<Token>(TK_CMD, data));
    }
  }
  return tokens_;
}

#include <iostream>
#include <memory>

#include "bfc.hpp"
#include "lexer.hpp"

const std::string kCorrectTokens = R"(<>+-.,[])";

Lexer::Lexer() {}

std::list<std::unique_ptr<Token>> Lexer::Lex(std::ifstream ifs) {
  char data;
  std::list<std::unique_ptr<Token>> tokens{};
  while (ifs.get(data)) {
    // ignore all non-token characters
    if (kCorrectTokens.find(data) != std::string::npos) {
      tokens.push_back(std::make_unique<Token>(TK_CMD, data));
    }
  }
  return tokens;
}

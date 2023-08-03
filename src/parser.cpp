#include <cassert>
#include <iostream>

#include "parser.hpp"

Parser::Parser(): token_iter_{}, token_end_{}, code_{} {}

std::list<std::unique_ptr<Node>>& Parser::Parse(const std::list<std::unique_ptr<Token>>& tokens) {
  token_iter_ = tokens.begin();
  token_end_ = tokens.end();
  Program();
  return code_;
}

// <program> ::= <command>*
void Parser::Program() {
  for (; token_iter_ != token_end_; ++token_iter_) {
    code_.push_back(Command());
  }
}

// <command> ::= '>' | '<' | '+' | '-' | '.' | ',' | '[' <command>* ']'
std::unique_ptr<Node> Parser::Command() {
  if (token_iter_ == token_end_ && depth_ != 0) {
    std::cerr << "Loop unclosed" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  switch ((*token_iter_)->value) {
    case '>':
      return std::make_unique<Node>(ND_INC_PTR);
    case '<':
      return std::make_unique<Node>(ND_DEC_PTR);
    case '+':
      return std::make_unique<Node>(ND_INC_VAL);
    case '-':
      return std::make_unique<Node>(ND_DEC_VAL);
    case '.':
      return std::make_unique<Node>(ND_PUT_CHAR);
    case ',':
      return std::make_unique<Node>(ND_GET_CHAR);
    case '[': {
      auto node = std::make_unique<Node>(ND_LOOP_START);
      ++depth_;
      for (;;) {
        ++token_iter_;
        auto child = Command();
        NodeKind kind = child->kind;
        node->children.push_back(std::move(child));
        if (kind == ND_LOOP_END) {
          --depth_;
          break;
        }
      }
      return node;
    }
    case ']':
      if (depth_ == 0) {
        std::cerr << "Invalid token in Command: " << (*token_iter_)->value << std::endl;
        std::exit(EXIT_FAILURE);
      }
      return std::make_unique<Node>(ND_LOOP_END);
    default:
      assert(false);
  }
}

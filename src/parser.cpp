#include <cassert>
#include <iostream>

#include "parser.hpp"

Parser::Parser(): tokens_{}, token_iter_{}, code_{} {}

std::list<std::shared_ptr<Node>>& Parser::Parse(std::list<std::shared_ptr<Token>>& tokens) {
  tokens_ = tokens;
  token_iter_ = tokens_.begin();
  Program();
  return code_;
}

// <program> ::= <command>*
void Parser::Program() {
  for (; token_iter_ != tokens_.end(); ++token_iter_) {
    std::shared_ptr<Node> node = Command();
    code_.push_back(node);
  }
}

// <command> ::= '>' | '<' | '+' | '-' | '.' | ',' | '[' <command>* ']'
std::shared_ptr<Node> Parser::Command() {
  if (token_iter_ == tokens_.end() && depth_ != 0) {
    std::cerr << "Loop unclosed" << std::endl;
    std::exit(EXIT_FAILURE);
  }

  switch ((*token_iter_)->value) {
    case '>':
      return std::make_shared<Node>(ND_INC_PTR);
    case '<':
      return std::make_shared<Node>(ND_DEC_PTR);
    case '+':
      return std::make_shared<Node>(ND_INC_VAL);
    case '-':
      return std::make_shared<Node>(ND_DEC_VAL);
    case '.':
      return std::make_shared<Node>(ND_PUT_CHAR);
    case ',':
      return std::make_shared<Node>(ND_GET_CHAR);
    case '[': {
      std::shared_ptr<Node> node = std::make_shared<Node>(ND_LOOP_START);
      ++depth_;
      for (;;) {
        ++token_iter_;
        std::shared_ptr<Node> child = Command();
        node->children.push_back(child);
        if (child->kind == ND_LOOP_END) {
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
      return std::make_shared<Node>(ND_LOOP_END);
    default:
      assert(false);
  }
}

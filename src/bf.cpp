#include <iostream>
#include <memory>
#include <vector>
#include <filesystem>
#include <fstream>
#include <cassert>

#include "bf.hpp"

Compiler::Compiler() : tokens_{}, token_iter_{}, code_{}, depth_{0} {}

void Compiler::GetCodeC(const std::string path) {
  Lex(path);
  Parse();
  PrintC();
}

std::ifstream Compiler::Load(const std::string path) {
  if (!std::filesystem::is_regular_file(path)) {
    std::cerr << path << " is not a regular file" << std::endl;
    std::exit(EXIT_FAILURE);
  }
  std::ifstream ifs{path, std::ios::binary | std::ios::in};
  if (!ifs.is_open()) {
    std::cerr << "Failed to open " << path << std::endl;
    std::exit(EXIT_FAILURE);
  }
  return ifs;
}

void Compiler::Lex(const std::string path) {
  auto ifs = Load(path);
  char data;
  while (ifs.get(data)) {
    // ignore all non-token characters
    if (kCorrectTokens.find(data) != std::string::npos) {
      tokens_.push_back(std::make_shared<Token>(TK_CMD, data));
    }
  }
  token_iter_ = tokens_.begin();
}

void Compiler::Parse() {
  Program();
}

// <program> ::= <command>*
void Compiler::Program() {
  for (; token_iter_ != tokens_.end(); ++token_iter_) {
    std::shared_ptr<Node> node = Command();
    code_.push_back(node);
  }
}

// <command> ::= '>' | '<' | '+' | '-' | '.' | ',' | '[' <command>* ']'
std::shared_ptr<Node> Compiler::Command() {
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
      std::shared_ptr node = std::make_shared<Node>(ND_LOOP_START);
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

void Compiler::GenerateC(std::shared_ptr<Node> node, int depth) {
  std::cout << "    ";
  if (node->kind == ND_LOOP_END) {
    for (int i = 0; i < depth - 1; ++i) {
      std::cout << "    ";
    }
  } else {
    for (int i = 0; i < depth; ++i) {
      std::cout << "    ";
    }
  }

  switch (node->kind) {
    case ND_INC_PTR:
      std::cout << "++ptr;" << std::endl;
      break;
    case ND_DEC_PTR:
      std::cout << "--ptr;" << std::endl;
      break;
    case ND_INC_VAL:
      std::cout << "++(*ptr);" << std::endl;
      break;
    case ND_DEC_VAL:
      std::cout << "--(*ptr);" << std::endl;
      break;
    case ND_PUT_CHAR:
      std::cout << "putchar(*ptr);" << std::endl;
      break;
    case ND_GET_CHAR:
      std::cout << "*ptr = getchar();" << std::endl;
      break;
    case ND_LOOP_START:
      std::cout << "while (*ptr) {" << std::endl;
      for (auto& child : node->children) {
        GenerateC(child, depth + 1);
      }
      break;
    case ND_LOOP_END:
      std::cout << "}" << std::endl;
      break;
    default:
      assert(false);
  }
}

void Compiler::PrintC() {
  std::cout << "#include <stdio.h>\n"
            << "#include <stdlib.h>\n\n"
            << "int main(void) {\n"
            << "    char *ptr = (char *)calloc(" << kMemSize << ", sizeof(char));\n" // calloc
            << "    if (ptr == NULL) {\n"
            << "        printf(\"Failed to allocate memory\");\n"
            << "        return 1;\n"
            << "    }\n" << std::endl;
  for (auto& code : code_) {
    GenerateC(code, 0);
  }
  std::cout << "\n    return 0;\n"
            << "}" << std::endl;
}

#include <cassert>
#include <iostream>
#include <memory>

#include "gen.hpp"

Generator::Generator() : code_{} {}

void Generator::PrintC(std::list<std::shared_ptr<Node>>& code) {
  code_ = code;
  std::cout
  << "#include <stdio.h>\n"
  << "#include <stdlib.h>\n\n"
  << "int main(void) {\n"
  << "    char *ptr = (char *)calloc(" << kMemSize << ", sizeof(char));\n" // calloc
  << "    if (ptr == NULL) {\n"
  << "        printf(\"Failed to allocate memory\");\n"
  << "        return 1;\n"
  << "    }\n"
  << std::endl;
  for (auto& code : code_) {
    GenerateC(code, 0);
  }
  std::cout
  << "\n    return 0;\n"
  << "}"
  << std::endl;
}

void Generator::GenerateC(std::shared_ptr<Node> node, int depth) {
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

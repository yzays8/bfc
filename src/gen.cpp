#include <cassert>
#include <iostream>
#include <memory>

#include "gen.hpp"

Generator::Generator() : label_{4} {}

void Generator::PrintC(const std::list<std::unique_ptr<Node>>& codes) {
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
  for (const auto& code : codes) {
    GenerateC(code, 0);
  }
  std::cout
  << "\n    return 0;\n"
  << "}"
  << std::endl;
}

void Generator::GenerateC(const std::unique_ptr<Node>& node, int depth) {
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
      for (const auto& child : node->children) {
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

void Generator::PrintX64(const std::list<std::unique_ptr<Node>>& codes) {
  std::cout
  << ".intel_syntax noprefix\n"
  << ".globl main\n"
  << ".LC0:\n"
  << "    .string \"Failed to allocate memory\"\n"
  << "main:\n"
  << "    push rbp\n"
  << "    mov rbp, rsp\n"
  << "    sub rsp, 16\n"
  << "    mov esi, 1\n" // sizeof(char)
  << "    mov edi, " << kMemSize << "\n"
  << "    call calloc\n"
  << "    mov QWORD PTR [rbp-8], rax\n" // rax is 64bits char pointer as return value of calloc
  << "    cmp QWORD PTR [rbp-8], 0\n"
  << "    jne .L2\n"
  << "    mov edi, OFFSET FLAT:.LC0\n"
  << "    call puts\n"
  << "    mov eax, 1\n"
  << "    jmp .L3\n"
  << ".L2:"
  << std::endl;
  for (const auto& code : codes) {
    GenerateX64(code);
  }
  std::cout
  << "    mov eax, 0\n" // return 0
  << ".L3:\n"
  << "    leave\n"
  << "    ret"
  << std::endl;
}

void Generator::GenerateX64(const std::unique_ptr<Node>& node) {
  switch (node->kind) {
    case ND_INC_PTR:
      std::cout << "    add QWORD PTR [rbp-8], 1" << std::endl;
      break;
    case ND_DEC_PTR:
      std::cout << "    sub QWORD PTR [rbp-8], 1" << std::endl;
      break;
    case ND_INC_VAL:
      std::cout
      << "    mov rax, QWORD PTR [rbp-8]\n"
      << "    movzx eax, BYTE PTR [rax]\n"
      << "    add eax, 1\n"
      << "    mov edx, eax\n"
      << "    mov rax, QWORD PTR [rbp-8]\n"
      << "    mov BYTE PTR [rax], dl"
      << std::endl;
      break;
    case ND_DEC_VAL:
      std::cout
      << "    mov rax, QWORD PTR [rbp-8]\n"
      << "    movzx eax, BYTE PTR [rax]\n"
      << "    sub eax, 1\n"
      << "    mov edx, eax\n"
      << "    mov rax, QWORD PTR [rbp-8]\n"
      << "    mov BYTE PTR [rax], dl"
      << std::endl;
      break;
    case ND_PUT_CHAR:
      std::cout
      << "    mov rax, QWORD PTR [rbp-8]\n"
      << "    movzx eax, BYTE PTR [rax]\n"
      << "    movsx eax, al\n"
      << "    mov edi, eax\n"
      << "    call putchar" // int putchar(int)
      << std::endl;
      break;
    case ND_GET_CHAR:
      std::cout
      << "    call getchar\n" // int getchar(void)
      << "    mov edx, eax\n"
      << "    mov rax, QWORD PTR [rbp-8]\n"
      << "    mov BYTE PTR [rax], dl"
      << std::endl;
      break;
    case ND_LOOP_START: {
      int this_label = label_;
      label_ += 2;
      std::cout
      << "    jmp .L" << this_label << '\n'
      << ".L" << this_label + 1 << ":"
      << std::endl;

      for (const auto& child : node->children) {
        GenerateX64(child);
      }

      std::cout
      << ".L" << this_label << ":\n"
      << "    mov rax, QWORD PTR [rbp-8]\n"
      << "    movzx eax, BYTE PTR [rax]\n"
      << "    test al, al\n"
      << "    jne .L" << this_label + 1
      << std::endl;
      break;
    }
    case ND_LOOP_END:
      break;
    default:
      assert(false);
  }
}

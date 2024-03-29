#include <iostream>
#include <cstdlib>
#include <cstring>
#include <sys/mman.h>

#include "front/lexer.h"
#include "front/parser.h"
#include "opt/pass_manager.h"
#include "mid/walker/analyzer/sema.h"
#include "mid/walker/irbuilder/irbuilder.h"

using namespace RJIT::front;
using namespace RJIT::mid;
using namespace RJIT::mid::analyzer;
using namespace RJIT::opt;

int main(int argc, char *argv[]) {
  std::string file = argv[1];
  Lexer lexer(file);
  Parser parser(&lexer);
  parser.Parse();

  SemAnalyzer semAnalyzer(parser.ast());
  semAnalyzer.Analyze();

//  parser.DumpAST();

  IRBuilder irBuilder(parser.ast());

  irBuilder.EmitIR();

  PassManager::Initialize();
  PassManager::SetModule(irBuilder.module());
  PassManager::RunPasses();
  irBuilder.module().Dump(std::cout);

//
//    RJIT::lib::Nested::NestedMapPtr<int, int *> ptr = MakeNestedMap<int, int *>();
//    for (int i = 1; i < 10; i++) {
//    int *value = new int(3*i);
////        *value = 3 * i;
//        std::cout << "value: " << *value << std::endl;
//        ptr->AddItem(i, value);
//    }

//    for (int i = 1; i < 10; i++) {
////        auto v = ptr->GetItem(i, true);
//        auto v = ptr->GetItem(i, true);
//        std::cout << i << ": " << *v << std::endl;
//    }
//
//    RJIT::lib::Nested::NestedMapPtr<int, int *> ptr1 = MakeNestedMap<int, int *>(ptr);
//    for (int i = 10; i < 20; i++) {
//        int *value = new int(4*i);
//        std::cout << "value: " << *value << std::endl;
//        ptr1->AddItem(i, value);
//    }
//    for (int i = 10; i < 20; i++) {
////        auto v = ptr->GetItem(i, true);
//        auto v = ptr1->GetItem(i, true);
//        std::cout << i << ": " << *v << std::endl;
//    }
//    for (int i = 1; i < 21; i++) {
////        auto v = ptr->GetItem(i, true);
//        auto v = ptr1->GetItem(i, true);
//        std::cout << i << ": " << *v << std::endl;
//    }
}

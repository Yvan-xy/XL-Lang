#include <iostream>

#include "opt/pass.h"
#include "lib/debug.h"
#include "mid/ir/castssa.h"
#include "opt/pass_manager.h"

int BlockCalculate;

namespace RJIT::opt {

/*
 * Calculate the predecessors and successors
 */
class BlockCalculate : public FunctionPass {
public:
  bool runOnFunction(const FuncPtr &F) final {

    for (const auto &it : *F) {
      auto block = std::static_pointer_cast<BasicBlock>(it.get());
      DBG_ASSERT(block != nullptr, "cast to block failed");

      // clear existed result
      block->preds().clear();
      block->succs().clear();
    }

    for (const auto &it : *F) {
      auto block = std::static_pointer_cast<BasicBlock>(it.get());
      DBG_ASSERT(block != nullptr, "cast to block failed");

      auto br = *(--block->inst_end());
      DBG_ASSERT(br != nullptr, "get branch instruction failed");
      auto inst = std::static_pointer_cast<Instruction>(br);
      DBG_ASSERT(inst != nullptr, "cast branch instruction failed");
      auto opcode = inst->opcode();

      // handle jump, branch and return
      if (opcode == Instruction::TermOps::Jmp) {
        auto jmp_inst = std::static_pointer_cast<JumpInst>(br);
        DBG_ASSERT(jmp_inst != nullptr, "cast jump instruction failed");
        auto target = jmp_inst->target();
        auto target_block = std::static_pointer_cast<BasicBlock>(target);
        block->AddSuccessor(target_block);
        target_block->AddPredecessor(block);
      } else if (opcode == Instruction::TermOps::Br) {
        auto br_inst = std::static_pointer_cast<BranchInst>(br);
        DBG_ASSERT(br_inst != nullptr, "cast branch instruction failed");
        auto true_block = std::static_pointer_cast<BasicBlock>(br_inst->true_block());
        auto false_block = std::static_pointer_cast<BasicBlock>(br_inst->false_block());
        block->AddSuccessor(true_block);
        block->AddSuccessor(false_block);
        true_block->AddPredecessor(block);
        false_block->AddPredecessor(block);
      } else if (opcode == Instruction::TermOps::Ret){
//        auto exit = CastTo<BasicBlock>((*F)[1].get());
//        exit->AddPredecessor(block);
//        block->AddSuccessor(exit);
      }
    }
    return false;
  }
};

class BlockCalculateFactory : public PassFactory {
public:
  PassInfoPtr CreatePass(PassManager *) override {
    auto pass = std::make_shared<BlockCalculate>();
    return std::make_shared<PassInfo>(pass, "BlockCalculate", false, 0);
  }
};

static PassRegisterFactory<BlockCalculateFactory> registry;

}
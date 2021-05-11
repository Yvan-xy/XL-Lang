#include <iostream>
#include <algorithm>

#include "opt/pass.h"
#include "lib/debug.h"
#include "mid/ir/castssa.h"
#include "opt/pass_manager.h"

int BlockMerge;

namespace RJIT::opt {

/*
  merge blocks that connected with jump instructions

  e.g.
    %0:
      ...                         %0:
      jump %1                       ...
    %1: ; preds: %0                 ...
      ...                 ==>>      jump %2
      jump %2                     %2: ; preds: %0, %3
    %2: ; preds: %1, %3             ...
      ...                           jump %4
      jump %4

*/
class BlockMerge : public FunctionPass {
private:
  bool _changed;

public:
  bool runOnFunction(const FuncPtr &F) final {
    _changed = false;
    SSAPtr entry;
    for (auto &it : *F) {
      if (entry == nullptr) entry = it.get();
      auto block = CastTo<BasicBlock>(it.get());

      if (block->preds().size() == 1) {
        auto pred = *(block->preds().begin());
        if (pred->succs().size() == 1) {
          _changed = true;

          // merge two blocks
          MergeBlocks(pred, block);
          block->ReplaceBy(pred);
          it.set(nullptr);
        }
      }

      // delete unreachable block
      if (block->preds().empty() && block != entry) {
        for (auto &succ : block->succs()) {
          auto &s_preds = succ->preds();
          s_preds.erase(std::remove_if(s_preds.begin(), s_preds.end(),
            [&block](const BlockPtr &B) {
              return B == block;
            }), s_preds.end());
        }
        it.set(nullptr);
      }
    }

    // remove all useless blocks
    F->RemoveValue(nullptr);
    // move entry to the head of list
    for (std::size_t i = 0; i < F->size(); i++) {
      if ((*F)[i].get() == entry) {
        if (i) {
          (*F)[i].set((*F)[0].get());
          (*F)[0].set(entry);
        }
        break;
      }
    }

    return _changed;
  }

  void MergeBlocks(const BlockPtr &pred, const BlockPtr &succ) {

    auto &insts = pred->insts();
    // remove jump instruction
    insts.pop_back();


    // move successor's instructions into predecessor
    insts.insert(pred->inst_end(), succ->inst_begin(), succ->inst_end());

    // remove successor from predecessor
    pred->succs().clear();

    // add succ's successors into pred
    for (const auto &it : succ->succs()) {
      pred->succs().push_back(it);

      // erase current succ from its successors
      auto &preds = it->preds();
      if (!preds.empty()) {
        for (auto block = preds.begin(); block != preds.end(); block++)  {
          if (*block == succ) {
            block = preds.erase(block);
            break;
          }
        }
      }

      // add new pred to succ's successors
      it->preds().push_back(pred);
    }
    succ->preds().clear();
    succ->succs().clear();
  }
};

class BlockMergeFactory : public PassFactory {
public:
  PassInfoPtr CreatePass(PassManager *) override {
    auto pass = std::make_shared<BlockMerge>();
    auto passinfo =  std::make_shared<PassInfo>(pass, "BlockMerge", false, false);
    passinfo->Requires("BlockCalculate");
    return passinfo;
  }
};

static PassRegisterFactory<BlockMergeFactory> registry;

}
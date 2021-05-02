#include "irbuilder.h"
#include "mid/ir/ssa.h"

namespace RJIT::mid {

SSAPtr IRBuilder::visit(IntAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(CharAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(StringAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(VariableAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(VariableDecl *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(VariableDefAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(BinaryStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(UnaryStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(ReturnStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(BreakStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(ContinueStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(CompoundStmt *node) {
  // make new environment when not in function
  auto guard = !_in_func ? NewEnv() : Guard(nullptr);
  if (_in_func) _in_func = false;

  // create new block
  const auto &cur_func = _module.InsertPoint()->parent();
  auto block = _module.CreateBlock(cur_func);
  _module.CreateJump(block);
  _module.SetInsertPoint(block);
  for (const auto &it : node->stmts()) {
    it->CodeGeneAction(this);
  }
  return block;
}

SSAPtr IRBuilder::visit(IfElseStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(CallStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(ProtoTypeAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(FunctionDefAST *node) {
  auto context = _module.SetContext(node->Logger());

  // make new environment
  auto env = NewEnv();
  _in_func = true;

  // generate prototype
  auto prototype = node->getProtoType()->CodeGeneAction(this);
  // generate body
  auto body = node->getBody()->CodeGeneAction(this);

  return nullptr;
}

SSAPtr IRBuilder::visit(FuncParamAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(PrimTypeAST *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(WhileStmt *node) {
  return nullptr;
}

SSAPtr IRBuilder::visit(TranslationUnitDecl *node) {
  for (const auto &it : node->Decls()) {
    it->CodeGeneAction(this);
  }

  return nullptr;
}

}
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
  std::string info;
  auto context = _module.SetContext(node->Logger());

  // generate function prototype
  FuncPtr func;
  auto functions = _module.Functions();
  auto func_name = node->getFuncName();
  auto func_type = node->AstType();
  if (!_module.GetFunction(node->getFuncName())) {
    func = _module.CreateFunction(func_name, func_type);
    functions.push_back(func);

    // add to environment
    auto symbols = _module.ValueSymTab();
    const auto &sym_table = _in_func ? symbols->outer() : symbols;
    sym_table->AddItem(func_name, func);
  } else {
    info = "function exist";
    return LogError(node->Logger(), info);
  }

  // create entry block to init the parameters
  auto entry = _module.CreateBlock(func, "entry");
  _module.SetFuncEntry(entry);
  _module.SetInsertPoint(entry);
  std::size_t index = 0;
  for (const auto &it : node->getArgs()) {
    auto param_alloc = it->CodeGeneAction(this);
    auto arg_ref = _module.CreateArgRef(func, index++);
    _module.CreateStore(arg_ref, param_alloc);
  }

  // create func_exit block to init the return value
  auto ret_type = func_type->GetReturnType();
  auto ret_val = (ret_type->IsVoid()) ? nullptr : _module.CreateAlloca(ret_type);
  _module.SetRetValue(ret_val);

  // jump to exit
  auto func_exit = _module.CreateBlock(func, "func_exit");
  _module.SetFuncExit(func_exit);
  _module.CreateJump(func_exit);
  _module.SetInsertPoint(func_exit);
  if (ret_val) {
    auto ret = _module.CreateLoad(ret_val);
    _module.CreateReturn(ret);
  } else {
    _module.CreateReturn(nullptr);
  }
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
  auto guard = NewEnv();
  for (const auto &it : node->Decls()) {
    it->CodeGeneAction(this);
  }

  return nullptr;
}

// print error message
SSAPtr IRBuilder::LogError(const front::LoggerPtr &log,
                           std::string &message, const std::string &id) {
  log->LogError(message, id);
  return nullptr;
}

SSAPtr IRBuilder::LogError(const front::LoggerPtr &log, std::string &message) {
  log->LogError(message);
  return nullptr;
}

}
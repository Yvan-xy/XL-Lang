#include <cassert>
#include "parser.h"
#include "lib/debug.h"
#include "define/type.h"
#include "define/color.h"

using namespace RJIT::TYPE;

namespace RJIT::front {
  static const std::string UnaryOperator[] = {"!", "-", "@", "$", "%", "^", "~"};

  void Parser::setPrecedence() {
    BinopPrecedence["="] = 2;
    BinopPrecedence["+="] = 2;
    BinopPrecedence["-="] = 2;
    BinopPrecedence["*="] = 2;
    BinopPrecedence["/="] = 2;
    BinopPrecedence["%="] = 2;
    BinopPrecedence["&="] = 2;
    BinopPrecedence["|="] = 2;
    BinopPrecedence["^="] = 2;
    BinopPrecedence["<<="] = 2;
    BinopPrecedence[">>="] = 2;
    BinopPrecedence["||"] = 4;
    BinopPrecedence["&&"] = 6;
    BinopPrecedence["|"] = 8;
    BinopPrecedence["^"] = 10;
    BinopPrecedence["&"] = 12;
    BinopPrecedence["<"] = 14;
    BinopPrecedence[">"] = 14;
    BinopPrecedence[">="] = 14;
    BinopPrecedence["<="] = 14;
    BinopPrecedence["=="] = 14;
    BinopPrecedence["!="] = 14;
    BinopPrecedence["<<"] = 16;
    BinopPrecedence[">>"] = 16;
    BinopPrecedence["+"] = 20;
    BinopPrecedence["-"] = 20;
    BinopPrecedence["*"] = 30;
    BinopPrecedence["/"] = 30;
    BinopPrecedence["%"] = 30;
  }

  int Parser::getPrecedence() {
    if (curToken.getTokenType() != TokenType::Operator) return -1;
    int prec = BinopPrecedence[curToken.getOperValue()];
    if (prec <= 0) return -1;
    return prec;
  }

  bool Parser::isIncrement() {
    if (curToken.isOper() && curToken.getOperValue() == "++") {
      return true;
    }
    return false;
  }

  bool Parser::isDecrement() {
    if (curToken.isOper() && curToken.getOperValue() == "--") {
      return true;
    }
    return false;
  }

  bool Parser::isLeftBrace() {
    if (curToken.isOper() && curToken.getOperValue() == "{") {
      return true;
    }
    return false;
  }

  bool Parser::isRightBrace() {
    if (curToken.isOper() && curToken.getOperValue() == "}") {
      return true;
    }
    return false;
  }

  bool Parser::isLeftParentheses() {
    if (curToken.isOper() && curToken.getOperValue() == "(") {
      return true;
    }
    return false;
  }

  bool Parser::isRightParentheses() {
    if (curToken.isOper() && curToken.getOperValue() == ")") {
      return true;
    }
    return false;
  }

  bool Parser::isComma() {
    if (curToken.isOper() && curToken.getOperValue() == ",") {
      return true;
    }
    return false;
  }

  bool Parser::isColon() {
    if (curToken.isOper() && curToken.getOperValue() == ":") {
      return true;
    }
    return false;
  }

  bool Parser::isSemicolon() {
    if (curToken.isOper() && curToken.getOperValue() == ";") {
      return true;
    }
    return false;
  }

  bool Parser::isEqualSign() {
    if (curToken.isOper() && curToken.getOperValue() == "=") {
      return true;
    }
    return false;
  }

  bool Parser::isDefine() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "def") {
      return true;
    }
    return false;
  }

  bool Parser::isExtern() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "extern") {
      return true;
    }
    return false;
  }

  bool Parser::isIf() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "if") {
      return true;
    }
    return false;
  }

  bool Parser::isThen() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "then") {
      return true;
    }
    return false;
  }

  bool Parser::isElse() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "else") {
      return true;
    }
    return false;
  }

  bool Parser::isFor() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "for") {
      return true;
    }
    return false;
  }

  bool Parser::isWhile() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "while") {
      return true;
    }
    return false;
  }

  bool Parser::isReturn() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "return") {
      return true;
    }
    return false;
  }


  bool Parser::isUnary() {
    if (curToken.isOper()) {
      std::string op = curToken.getOperValue();
      for (const auto &i : UnaryOperator) {
        if (i == op) {
          return true;
        }
      }
    }
    return false;
  }

  bool Parser::isConst() {
    if (curToken.isChar() || curToken.isInt() || curToken.isString()) {
      return true;
    }
    return false;
  }

  bool Parser::isIn() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "in") {
      return true;
    }
    return false;
  }

  bool Parser::isEnd() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "end") {
      return true;
    }
    return false;
  }

  bool Parser::isVar() {
    if (curToken.isKeyword() &&
        curToken.getKeywordValue() == "var") {
      return true;
    }
    return false;
  }

  TYPE::Type Parser::getType() {
    if (curToken.isKeyword()) {
      Type type = TYPE::string2Type(curToken.getKeywordValue());
      if (type != Type::Dam) {
        return type;
      }
    }
    return Type::Dam;
  }

  ASTPtr Parser::LogError(const std::string &info) {
    auto red = Color::Modifier(Color::Code::FG_RED);
    auto def = Color::Modifier(Color::Code::FG_DEFAULT);
    std::cerr << lexer->GetFilename() << ":"
              << curToken.getLineNumber() << ":"
              << curToken.getPosition() << ": "
              << red << "error: " << def
              << info << std::endl;
    return nullptr;
  }


  ASTPtr Parser::ParseVariableDecl() {
    nextToken();// eat var
    ASTPtr variableDefAst;
    ASTPtrList defs;

    if (!curToken.isIdentifier()) {
      return LogError("Expect identifier here.");
    }

    do {
      variableDefAst = ParseVariableDefine();
      if (!variableDefAst) { return nullptr; }
      defs.push_back(std::move(variableDefAst));
    } while (curToken.isIdentifier());

    nextToken();// eat :

    auto log = logger();
    auto type = string2Type(curToken.getKeywordValue());
    PrimASTPtr typeASTPtr = MakePrimeAST(std::move(log), type);

    nextToken();// eat type
    if (!isSemicolon()) {
      LogError("Expect a ; here.");
    }
    nextToken();// eat ;

    log = logger();
    return MakeAST<VariableDecl>(
        std::move(log), std::move(typeASTPtr), std::move(defs));
  }

  ASTPtr Parser::ParseVariableDefine() {
    std::string identifier;
    ASTPtr initValue = nullptr;
    auto log = logger();
    if (!curToken.isIdentifier()) {
      return LogError("Expect identifier here.");
    }

    // Get variable name
    identifier = curToken.getIdentifierValue();
    nextToken();// eat identifier

    // check init value
    if (isEqualSign()) {
      nextToken();// eat =
      switch (curToken.getTokenType()) {
        case TokenType::Int:
          initValue = MakeAST<IntAST>(std::move(log), curToken.getIntValue());
          break;
        case TokenType::Char:
          initValue = MakeAST<CharAST>(std::move(log), curToken.getCharValue());
          break;
        case TokenType::String:
          initValue = MakeAST<StringAST>(std::move(log), curToken.getStringValue());
          break;
        default:
          return LogError("Expect a int, char, string here.");
      }
      nextToken();// eat init value
    }

    // var a = 1, b : int;  Expect a comma or colon here.
    if (!isComma() && !isColon()) {
      return LogError("Expect a comma or colon here.");
    }

    // eat ,
    if (isComma()) { nextToken(); }

    log = logger();
    return MakeAST<VariableDefAST>(std::move(log), identifier, std::move(initValue));
  }

  ASTPtr Parser::ParseParenExpr() {
    nextToken();// eat (
    ASTPtr expr = ParseExpression();
    if (!expr) return nullptr;
    if (!isRightParentheses()) {
      LogError("Expect ')' here.");
    }
    nextToken();// eat ')'
    return expr;
  }

  ASTPtr Parser::ParseUnary() {
    auto log = logger();
    if (isUnary()) {
      std::string op = curToken.getOperValue();
      nextToken();// eat operator
      if (auto operand = ParseUnary()) {
        return MakeAST<UnaryStmt>(std::move(log), string2Operator(op, true), std::move(operand));
      }
    } else {
      return ParsePrimary();
    }
    return LogError("Parse Unary Failed.");
  }

  ASTPtr Parser::ParseExpression() {
    ASTPtr LHS = ParseUnary();
    if (!LHS) return nullptr;
    return ParseBinaryOPRHS(0, std::move(LHS));
  }

  ASTPtr Parser::ParseBinaryOPRHS(int prec, ASTPtr lhs) {
    while (true) {
      int tokPrec = getPrecedence();
      if (tokPrec < prec) return lhs;

      Operator op = string2Operator(curToken.getOperValue());
      nextToken();// eat operator

      ASTPtr rhs = ParseUnary();
      if (!rhs) return nullptr;

      int nextPrec = getPrecedence();
      if (tokPrec < nextPrec) {
        rhs = ParseBinaryOPRHS(tokPrec + 1, std::move(rhs));
        if (!rhs) return nullptr;
      }

      auto log = logger();
      lhs = MakeAST<BinaryStmt>(
          std::move(log), op, std::move(lhs), std::move(rhs));
    }
  }

  ASTPtr Parser::ParseIdentifier() {
    assert(curToken.isIdentifier());
    std::string identName = curToken.getIdentifierValue();
    nextToken();


    /* Parse variable */
    if (!isLeftParentheses()) {
      auto log = logger();
      ASTPtr varAST = MakeAST<VariableAST>(std::move(log), identName);
      if (!isEqualSign() && !isIncrement() && !isDecrement()) {
        return varAST;
      } else if (isIncrement()) {
        auto intVal = MakeAST<IntAST>(logger(), 1);

        nextToken(); // eat '++'

        return MakeAST<BinaryStmt>(std::move(log), Operator::Add, std::move(varAST), std::move(intVal));
      } else if (isDecrement()) {
        auto intVal = MakeAST<IntAST>(logger(), 1);

        nextToken(); // eat '--'

        return MakeAST<BinaryStmt>(std::move(log), Operator::Sub, std::move(varAST), std::move(intVal));
      } else if (isEqualSign()) {
        ASTPtr assignAST = ParseBinaryOPRHS(0, std::move(varAST));

        return assignAST;
      } else {
        DBG_WARN(1, "unknown operator here");
      }
    }

    /* Parse function call */
    return ParseFunctionCall(identName);
  }

  ASTPtr Parser::ParseFunctionCall() {
    assert(curToken.isIdentifier());
    std::string identName = curToken.getIdentifierValue();
    nextToken();

    return ParseFunctionCall(identName);
  }

  ASTPtr Parser::ParseFunctionCall(const std::string &func_name) {
    assert(isLeftParentheses());
    nextToken();// eat '('

    ASTPtrList args;
    ASTPtr argAST;
    if (!isRightParentheses()) {
      while (true) {
        argAST = ParseExpression();
        if (argAST) {
          args.push_back(std::move(argAST));
        } else {
          return nullptr;
        }

        if (isRightParentheses()) break;

        if (!isComma()) {
          LogError("Expect a ',' or ')' in arguments list.");
        }
        nextToken();
      }
    }
    nextToken();// eat ')'

    auto log = logger();
    return MakeAST<CallStmt>(std::move(log), func_name, std::move(args));
  }

  ASTPtr Parser::ParseFunctionDef() {
    nextToken();// eat "def"

    Type type;
    ASTPtrList args;
    ASTPtr funcDefAST, protoAST, argAST, blockAST;
    ASTPtr typeAST;
    std::string funcName, argName;
    LoggerPtr log;

    if (!curToken.isIdentifier()) {
      LogError("Expect function name in function Proto.");
    }

    funcName = curToken.getIdentifierValue();
    nextToken();// eat function name

    if (!isLeftParentheses()) {
      LogError("Expect a '(' in function proto.");
    }
    nextToken();// eat '('

    if (!isRightParentheses()) {
      while (true) {
        if (!curToken.isIdentifier()) {
          LogError("Expect a identifier in function arguments.");
        }

        argName = curToken.getIdentifierValue();
        nextToken();// eat argument name
        type = getType();

        if (type == Type::Dam) {
          LogError("Expect corrent type here.");
        }
        nextToken();// eat type

        log = logger();
        typeAST = MakeAST<PrimTypeAST>(std::move(log), type);

        log = logger();
        argAST = MakeAST<FuncParamAST>(std::move(log), std::move(typeAST), argName);
        args.push_back(std::move(argAST));

        if (isRightParentheses()) break;

        if (!isComma()) {
          LogError("Expect a ',' in function proto.");
        }
        nextToken();// eat ','
      }
    }
    nextToken();// eat )


    type = getType();
    if (curToken.getOperValue() != "{") {
      if (type == Type::Dam) {
        LogError("Expect a corrent type for function return.");
      }
      nextToken();// eat return type
    } else {
      type = Type::Void;
    }

    log = logger();
    protoAST = MakeAST<ProtoTypeAST>(std::move(log), funcName, std::move(args), type);


    blockAST = ParseBlock();
    if (!blockAST) return nullptr;

    log = logger();
    return MakeAST<FunctionDefAST>(std::move(log), std::move(protoAST), std::move(blockAST));
  }

  ASTPtr Parser::ParseIfElse() {
    nextToken();// eat 'if'
    ASTPtr cond = ParseExpression();
    if (!cond) return nullptr;

    ASTPtr then_ = ParseBlock(), else_ = nullptr;
    if (!then_) return nullptr;

    if (isElse()) {
      nextToken();// eat else
      else_ = ParseBlock();
      if (!else_) return nullptr;
    }

    auto log = logger();
    return MakeAST<IfElseStmt>(
        std::move(log), std::move(cond), std::move(then_), std::move(else_));
  }

  ASTPtr Parser::ParseWhile() {
    nextToken();// eat while
    ASTPtr cond, block;

    cond = ParseExpression();
    if (!cond) return nullptr;

    block = ParseBlock();
    if (!block) return nullptr;

    auto log = logger();
    return MakeAST<WhileStmt>(std::move(log), std::move(cond), std::move(block));
  }

  ASTPtr Parser::ParseBlock() {
    ASTPtr blockAST, expr;
    ASTPtrList exprs;
    if (!isLeftBrace()) {
      LogError("Expect '{' here.");
    }
    nextToken();// eat '{'

    if (!isRightBrace()) {
      while (true) {
        expr = ParsePrimary();
        if (!expr) return nullptr;
        exprs.push_back(std::move(expr));
        if (isRightBrace()) break;
      }
    }
    nextToken();// eat '}'

    auto log = logger();
    blockAST = MakeAST<CompoundStmt>(std::move(log), std::move(exprs));
    return blockAST;
  }

  ASTPtr Parser::ParseConst() {
    ASTPtr constAST;
    auto log = logger();
    if (curToken.isString()) {
      constAST = MakeAST<StringAST>(std::move(log), curToken.getStringValue());
    } else if (curToken.isInt()) {
      constAST = MakeAST<IntAST>(std::move(log), curToken.getIntValue());
    } else if (curToken.isChar()) {
      constAST = MakeAST<CharAST>(std::move(log), curToken.getCharValue());
    }
    nextToken();// eat const value
    return constAST;
  }

  ASTPtr Parser::ParseReturn() {
    nextToken();// eat return
    auto retVal = ParseExpression();

    auto log = logger();
    ASTPtr retAST = MakeAST<ReturnStmt>(std::move(log), std::move(retVal));

    return retAST;
  }

  ASTPtr Parser::ParsePrimary() {
    ASTPtr node;
    if (isDefine()) {
      node = ParseFunctionDef();
    } else if (isVar()) {
      node = ParseVariableDecl();
    } else if (isIf()) {
      node = ParseIfElse();
    } else if (isWhile()) {
      node = ParseWhile();
    } else if (curToken.isIdentifier()) {
      node =  ParseIdentifier();
    } else if (isConst()) {
      node = ParseConst();
    } else if (isLeftParentheses()) {
      node = ParseParenExpr();
    } else if (isReturn()) {
      node = ParseReturn();
    } else {
      node = ParseExpression();
    }
    if (isSemicolon()) nextToken(); // eat ';'
    return node;
  }

  ASTPtr Parser::ParseTop() {
    if (!isDefine() && !isVar()) { return nullptr; }
    return ParsePrimary();
  }

  void Parser::Parse() {
    ASTPtrList defs;
    ASTPtr def;
    def = ParseTop();
    while (true) {
      if (!def) { break; }
      defs.push_back(std::move(def));
      def = ParseTop();
    }

    auto log = logger();
    rootNode = MakeAST<TranslationUnitDecl>(std::move(log), std::move(defs));
  }

}// namespace RJIT::front
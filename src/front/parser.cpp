#include "parser.h"
#include "define/type.h"
#include "define/color.h"

using namespace RJIT::TYPE;

namespace RJIT::front {
    static const std::string UnaryOperator[] = {
            "!", "-", "@", "$", "%", "^"
    };

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
        if (curToken.isChar() || curToken.isInt() || curToken.isInt()) {
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
        nextToken();    // eat var
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

        nextToken();    // eat :

        auto type = string2Type(curToken.getKeywordValue());
        ASTPtr typeASTPtr = std::make_unique<PrimTypeAST>(type);

        nextToken();    // eat type
        if (!isSemicolon()) {
            LogError("Expect a ; here.");
        }
        nextToken();    // eat ;

        return std::make_unique<VariableDeclAST>(
                std::move(typeASTPtr), std::move(defs));
    }

    ASTPtr Parser::ParseVariableDefine() {
        std::string identifier;
        ASTPtr initValue = nullptr;
        if (!curToken.isIdentifier()) {
            return LogError("Expect identifier here.");
        }

        // Get variable name
        identifier = curToken.getIdentifierValue();
        nextToken();    // eat identifier

        // check init value
        if (isEqualSign()) {
            nextToken();    // eat =
            switch (curToken.getTokenType()) {
                case TokenType::Int:
                    initValue = std::make_unique<IntAST>(curToken.getIntValue());
                    break;
                case TokenType::Char:
                    initValue = std::make_unique<CharAST>(curToken.getCharValue());
                    break;
                case TokenType::String:
                    initValue = std::make_unique<StringAST>(curToken.getStringValue());
                    break;
                default:
                    return LogError("Expect a int, char, string here.");
            }
            nextToken();    // eat init value
        }

        // var a = 1, b : int;  Expect a comma or colon here.
        if (!isComma() && !isColon()) {
            return LogError("Expect a comma or colon here.");
        }

        // eat ,
        if (isComma()) { nextToken(); }

        return std::make_unique<VariableDefAST>(identifier, std::move(initValue));
    }

    ASTPtr Parser::ParseParenExpr() {
        nextToken();    // eat (
        ASTPtr expr = ParseExpression();
        if (!expr) return nullptr;
        if (!isRightParentheses()) {
            LogError("Expect ')' here.");
        }
        nextToken();    // eat ')'
        return expr;
    }

    ASTPtr Parser::ParseUnary() {
        if (isUnary()) {
            std::string op = curToken.getOperValue();
            nextToken();    // eat operator
            if (auto operand = ParseUnary()) {
                return std::make_unique<UnaryAST>(op, std::move(operand));
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

    ASTPtr Parser::ParseBare() {
        auto expr = ParseExpression();
        if (!expr) return nullptr;

        if (!isSemicolon()) {
            LogError("Expect a ';' here.");
        }
        return expr;
    }

    ASTPtr Parser::ParseBinaryOPRHS(int prec, ASTPtr lhs) {
        while (true) {
            int tokPrec = getPrecedence();
            if (tokPrec < prec) return lhs;

            Operator op = string2Operator(curToken.getOperValue());
            nextToken();    // eat operator

            ASTPtr rhs = ParseUnary();
            if (!rhs) return nullptr;

            int nextPrec = getPrecedence();
            if (tokPrec < nextPrec) {
                rhs = ParseBinaryOPRHS(tokPrec + 1, std::move(rhs));
                if (!rhs) return nullptr;
            }
            lhs = std::make_unique<BinaryAST>(op, std::move(lhs), std::move(rhs));
        }
    }

    ASTPtr Parser::ParseIdentifier() {
        assert(curToken.isIdentifier());
        std::string identName = curToken.getIdentifierValue();
        nextToken();

        /* Parse variable */
        if (!isLeftParentheses()) {
            ASTPtr varAST = std::make_unique<VariableAST>(identName);
            if (!isEqualSign()) {
                return varAST;
            } else {
                ASTPtr assignAST = ParseBinaryOPRHS(0, std::move(varAST));
                if (!isSemicolon()) {
                    LogError("Expect a ';' here.");
                }
                nextToken();    // eat ;
                return assignAST;
            }
        }

        /* Parse function call */
        assert(isLeftParentheses());
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
        nextToken();    // eat ')'

        return std::make_unique<CallAST>(identName, std::move(args));
    }

    ASTPtr Parser::ParseFunctionDef() {
        nextToken();    // eat "def"

        Type type;
        ASTPtrList args;
        ASTPtr funcDefAST, protoAST, argAST, blockAST;
        PrimASTPtr typeAST;
        std::string funcName, argName;

        if (!curToken.isIdentifier()) {
            LogError("Expect function name in function Proto.");
        }

        funcName = curToken.getIdentifierValue();
        nextToken();    // eat function name

        if (!isLeftParentheses()) {
            LogError("Expect a '(' in function proto.");
        }
        nextToken();    // eat '('

        if (!isRightParentheses()) {
            while (true) {
                if (!curToken.isIdentifier()) {
                    LogError("Expect a identifier in function arguments.");
                }

                argName = curToken.getIdentifierValue();
                nextToken();    // eat argument name
                type = getType();

                if (type == Type::Dam) {
                    LogError("Expect corrent type here.");
                }
                nextToken();    // eat type

                typeAST = std::make_unique<PrimTypeAST>(type);
                argAST = std::make_unique<FuncParamAST>(std::move(typeAST), funcName);
                args.push_back(std::move(argAST));

                if (isRightParentheses()) break;

                if (!isComma()) {
                    LogError("Expect a ',' in function proto.");
                }
                nextToken();    // eat ','
            }
        }
        nextToken();    // eat )


        type = getType();
        if (curToken.getOperValue() != "{") {
            if (type == Type::Dam) {
                LogError("Expect a corrent type for function return.");
            }
            nextToken();    // eat return type
        } else {
            type = Type::Void;
        }

        protoAST = std::make_unique<ProtoTypeAST>(funcName, std::move(args), type);


        blockAST = ParseBlock();
        if (!blockAST) return nullptr;

        return std::make_unique<FunctionDefAST>(std::move(protoAST), std::move(blockAST));
    }

    ASTPtr Parser::ParseIfElse() {
        return RJIT::AST::ASTPtr();
    }

    ASTPtr Parser::ParseWhile() {
        return RJIT::AST::ASTPtr();
    }

    ASTPtr Parser::ParseBlock() {
        ASTPtr blockAST, expr;
        ASTPtrList exprs;
        if (!isLeftBrace()) {
            LogError("Expect '{' here.");
        }
        nextToken();    // eat '{'

        if (!isRightBrace()) {
            while (true) {
                expr = ParsePrimary();
                if (!expr) return nullptr;
                exprs.push_back(std::move(expr));
                if (isRightBrace()) break;
            }
        }
        nextToken();    // eat '}'

        blockAST = std::make_unique<BlockAST>(std::move(exprs));
        return blockAST;
    }

    ASTPtr Parser::ParseConst() {
        ASTPtr constAST;
        if (curToken.isString()) {
            constAST = std::make_unique<StringAST>(curToken.getStringValue());
        } else if (curToken.isInt()) {
            constAST = std::make_unique<IntAST>(curToken.getIntValue());
        } else if (curToken.isChar()) {
            constAST = std::make_unique<CharAST>(curToken.getCharValue());
        }
        nextToken();    // eat const value
        return constAST;
    }

    ASTPtr Parser::ParsePrimary() {
        if (isDefine()) {
            return ParseFunctionDef();
        } else if (isVar()) {
            return ParseVariableDecl();
        } else if (isIf()) {
            return ParseIfElse();
        } else if (isWhile()) {
            return ParseWhile();
        } else if (curToken.isIdentifier()) {
            return ParseIdentifier();
        } else if (isConst()) {
            return ParseConst();
        } else if (isLeftParentheses()) {
            return ParseParenExpr();
        }
        return nullptr;
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

        rootNode = std::make_unique<BlockAST>(std::move(defs));
    }

}
#include "parser.h"
#include "define/color.h"

namespace RJIT::front {
    static const std::string UnaryOperator[] = {
            "!", "-", "@", "$", "%", "^"
    };

    void Parser::setPrecedence() {
        BinopPrecedence["="] = 2;
        BinopPrecedence["<"] = 10;
        BinopPrecedence[">"] = 10;
        BinopPrecedence["+"] = 20;
        BinopPrecedence["-"] = 20;
        BinopPrecedence["*"] = 30;
        BinopPrecedence["/"] = 30;
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

    ASTPtr Parser::ParseParenExpr() {
        return RJIT::AST::ASTPtr();
    }

    ASTPtr Parser::ParseExpression() {
        return RJIT::AST::ASTPtr();
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
            defs.push_back(std::move(variableDefAst));
        } while (isSemicolon());

        return RJIT::AST::ASTPtr();
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

        return std::make_unique<VariableDefAST>(identifier, std::move(initValue));
    }

    ASTPtr Parser::ParsePrimary() {
        if (curToken.getTokenType() == TokenType::Identifier) {
            return ParseIdentifier();
        } else if (isVar()) {
            return ParseVariableDecl();
        }
    }


}
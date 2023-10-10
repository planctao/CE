//
// Created by me on 2023/9/19.
//

#include "utils.h"
#include "TokenType.h"
bool isNonDigit(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool isSingleSymbol(char c) {//判断是否是单符号，比如'+','-',',',各个括号之类的
    return c == '+' || c == '-' ||  c == '*' || c == '%' || c == ',' || c == ';' || c == '(' || c == ')' || c == '[' ||
           c == ']' || c == '{' || c == '}';
}

bool isCondSymbol(char c) {
    return c == '=' || c == '!' || c == '<' || c == '>';
}

TokenType string2TokenType(std::string str) {
#define strToType(s,type) \
    if (str == s) { \
        return TokenType::type;                \
    }

    strToType("main",MAINTK)
    strToType("const",CONSTTK)
    strToType("int",INTTK)
    strToType("break",BREAKTK)
    strToType("continue",CONTINUETK)
    strToType("if",IFTK)
    strToType("else",ELSETK)
    strToType("!",NOT)
    strToType("&&",AND)
    strToType("||",OR)
    strToType("for",FORTK)
    strToType("getint",GETINTTK)
    strToType("printf",PRINTFTK)
    strToType("return",RETURNTK)
    strToType("+",PLUS)
    strToType("-",MINU)
    strToType("void",VOIDTK)
    strToType("*",MULT)
    strToType("/",DIV)
    strToType("%",MOD)
    strToType("<",LSS)
    strToType("<=",LEQ)
    strToType(">",GRE)
    strToType(">=",GEQ)
    strToType("==",EQL)
    strToType("!=",NEQ)
    strToType("=",ASSIGN)
    strToType(";",SEMICN)
    strToType(",",COMMA)
    strToType("(",LPARENT)
    strToType("[",LBRACK)
    strToType("{",LBRACE)
    strToType(")",RPARENT)
    strToType("]",RBRACK)
    strToType("}",RBRACE)
    strToType("while",WHILETK)
    //接下来的是判断数字还是标识符
    if (isdigit(str[0])) {//如果合法只能是数字
        if(str[0] == '0') {
            if (str.size() > 1) {
                return TokenType::INVALIDTK;
            }
            else {
                return TokenType::INTCON;
            }
        }
        else {
            for (const auto & i : str) {
                if (!isdigit(i)) {}
                return TokenType ::INVALIDTK;
            }
            return TokenType::IDENFR;
        }
    }
    else if (isNonDigit(str[0])) {
        return TokenType::IDENFR;
    }

    return TokenType::UNKNOWED;
}






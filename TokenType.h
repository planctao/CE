//
// Created by me on 2023/9/18.
//

#ifndef COMPILER_TOKENTYPE_H
#define COMPILER_TOKENTYPE_H
#include<string>
enum class TokenType{
    UNKNOWED, //未知
    INVALIDTK,//非法

    IDENFR,//标识符
    INTCON,//常数
    STRCON,//formatString
    MAINTK,//main
    CONSTTK,//const
    INTTK,//int
    BREAKTK,//break
    CONTINUETK,//continue
    IFTK,//if
    ELSETK,//else
    NOT,// "!"
    AND,// "and"
    OR,// "or"
    FORTK,// "for"
    GETINTTK,// getint
    PRINTFTK,//printf
    RETURNTK,//return
    PLUS,// "+"
    MINU,// "-"
    VOIDTK,//void
    MULT,// "*"
    DIV,// "/"
    MOD, //"%"
    LSS, // "<"
    LEQ, //"<="
    GRE, //">"
    GEQ, //">="
    EQL, //"=="
    NEQ, // "!="
    ASSIGN, //"="
    SEMICN, //";"
    COMMA, //","
    LPARENT, //左括号
    RPARENT, //右括号
    LBRACK, //左中括号
    RBRACK, //右中括号
    LBRACE, //左大括号
    RBRACE, //右大括号
    WHILETK,
    NullType
};

const std::string typeStrings[] = {"UNKNOWED","INVALIDTK","IDENFR","INTCON","STRCON",
                                   "MAINTK","CONSTTK","INTTK","BREAKTK","CONTINUETK",
                                   "IFTK","ELSETK","NOT","AND","OR","FORTK","GETINTTK",
                                   "PRINTFTK","RETURNTK","PLUS","MINU","VOIDTK","MULT",
                                   "DIV","MOD","LSS","LEQ","GRE","GEQ","EQL","NEQ",
                                   "ASSIGN","SEMICN","COMMA","LPARENT","RPARENT",
                                   "LBRACK","RBRACK","LBRACE","RBRACE","WHILETK","NullType"};


#endif //COMPILER_TOKENTYPE_H

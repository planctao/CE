//
// Created by me on 2023/9/26.
//

#ifndef COMPILER_GRAMMARTYPE_H
#define COMPILER_GRAMMARTYPE_H
#include<string>
enum  class GrammarType{
    CompUnit,
    Decl,
    FuncDef,
    MainFuncDef,
    ConstDecl,
    VarDecl,
    ConstDef,
    BType,
    Ident,
    ConstExp,
    ConstInitVal,
    InitVal,
    Exp,
    FuncType,
    FuncFParams,
    Block,
    FuncFParam,
    BlockItem,
    Stmt,
    LVal,
    Cond,
    ForStmt,
    FormatString,
    AddExp,
    LOrExp,
    PrimaryExp,
    Number,
    IntConst,
    UnaryExp,
    UnaryOp,
    MulExp,
    RelExp,
    EqExp,
    LAndExp,
    VarDef,
    FuncRParams,
    NullType
};
const std::string grammarStrings[] = {"CompUnit","Decl","FuncDef","MainFuncDef","ConstDecl","VarDecl","ConstDef","BType",
                                "Ident","ConstExp","ConstInitVal","InitVal","Exp","FuncType","FuncFParams","Block",
                                "FuncFParam","BlockItem","Stmt","LVal","Cond","ForStmt","FormatString","AddExp",
                                "LOrExp","PrimaryExp","Number","IntConst","UnaryExp","UnaryOp","MulExp","RelExp",
                                "EqExp","LAndExp","VarDef","FuncRParams","NullType"};

#endif //COMPILER_GRAMMARTYPE_H

//
// Created by me on 2023/9/19.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H
#include "TokenType.h"
#include "../GrammarTree/Token.h"
#include <string>
#include "GrammarType.h"
#include "InstructionType.h"
bool isNonDigit(char c);
bool isSingleSymbol(char c);
bool isCondSymbol(char c);

TokenType string2TokenType(std::string str);

std::string GrammarType2String(GrammarType gt);
std::string genGlobalDef(bool if_const,std::string identName,int dim,std::vector<int> dimList,std::vector<std::vector<int>> valueList);
std::string genArrayStmt(std::string typeName,std::vector<int> valueList);
int getFuncCnt(int *cnt);
std::vector<std::vector<int>> zeroArray(int dim,std::vector<int>dimList);
int str2int(std::string value);
#define isDecl(x) (((x).getGrammarType()==GrammarType::Decl))
#define isStmt(x) (((x).getGrammarType()==GrammarType::Stmt))
#define isBlock(x) (((x).getGrammarType()==GrammarType::Block))
#define isFuncDef(x) (((x).getGrammarType()==GrammarType::FuncDef))
#define isConstDecl(x) (((x).getGrammarType()==GrammarType::ConstDecl))
#define isVarDecl(x) (((x).getGrammarType()==GrammarType::VarDecl))
#define isConstDef(x) (((x).getGrammarType()==GrammarType::ConstDef))
#define isVarDef(x) (((x).getGrammarType()==GrammarType::VarDef))
#define isInitVal(x) (((x).getGrammarType()==GrammarType::InitVal))
#define isLVal(x) (((x).getGrammarType()==GrammarType::LVal))
#define isExp(x) (((x).getGrammarType()==GrammarType::Exp))
#define isPrintf(x) (((x).getLeafTokenName()=="printf"))
#define isBlock(x) (((x).getGrammarType()==GrammarType::Block))
#define isReturn(x) (((x).getLeafTokenName()=="return"))
#define isIf(x) (((x).getLeafTokenName()=="if"))
#define isFor(x) (((x).getLeafTokenName()=="for"))
#define isBreak(x) (((x).getLeafTokenName()=="break"))
#define isContinue(x) (((x).getLeafTokenName()=="continue"))
#define isAddExp(x) (((x).getGrammarType()==GrammarType::AddExp))
#define isMulExp(x) (((x).getGrammarType()==GrammarType::MulExp))
#define isUnaryExp(x) (((x).getGrammarType()==GrammarType::UnaryExp))
#define isPrimaryExp(x) (((x).getGrammarType()==GrammarType::PrimaryExp))
#define isUnaryOp(x) (((x).getGrammarType()==GrammarType::UnaryOp))
#define isNumber(x) (((x).getGrammarType()==GrammarType::Number))
#define isGetint(x) (((x).getLeafTokenName()=="getint"))
#define isConstExp(x) (((x).getGrammarType()==GrammarType::ConstExp))
#define isConstInitVal(x) (((x).getGrammarType()==GrammarType::ConstInitVal))
#define isMainFuncDef(x) (((x).getGrammarType()==GrammarType::MainFuncDef))
#define isFuncRParams(x) (((x).getGrammarType()==GrammarType::FuncRParams))
#define isBlockItem(x) (((x).getGrammarType()==GrammarType::BlockItem))
#define gt2str(x) std::cout << GrammarType2String((x)) << '\n';
#define isForStmt(x) (((x).getGrammarType()==GrammarType::ForStmt))
#define isCond(x) (((x).getGrammarType()==GrammarType::Cond))

int strReg2intReg(std::string reg);
std::string intReg2strReg(int reg);

#define ADD_INTO_MIPS_TABLE(x) ((this->mipsSymbolTable->addMipsSymbol((x)))) ;
#endif //COMPILER_UTILS_H

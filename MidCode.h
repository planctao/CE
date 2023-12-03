//
// Created by me on 2023/10/28.
//
/*中间代码，现在目前暂时决定采用的是LLVM生成*/
#ifndef COMPILER_MIDCODE_H
#define COMPILER_MIDCODE_H
#include"./Symbol/SymbolTable.h"
#include"./GrammarTree/Node.h"
#include<string>
class MidCode {
private:
    SymbolTable* tableRoot;
    Node root;
    std::vector<std::string> llvmIR;
    static int curScope;
public:
    MidCode(Node root);


    void writeDecl(std::string identName,std::vector<int> dimList,std::vector<std::vector<int>> valueList,int kind,int scope);
    void visitCompUnit(Node* curNode);
    void visitDecl(Node* curNode,SymbolTable *curTable);
    void visitFuncDef(Node* curNode,SymbolTable *curTable);
    void visitMainFuncDef(Node* curNode,SymbolTable *curTable);
    void visitConstDecl(Node* curNode,SymbolTable *curTable);
    void visitVarDecl(Node* curNode,SymbolTable *curTable);
    void visitConstDef(Node* curNode,SymbolTable *curTable);
    void visitVarDef(Node* curNode,SymbolTable *curTable);
};


#endif //COMPILER_MIDCODE_H

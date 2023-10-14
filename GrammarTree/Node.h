//
// Created by me on 2023/9/26.
//

#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H
#include<vector>
#include<iostream>
#include<map>
#include <utility>
#include"Token.h"
#include"../Util/GrammarType.h"
#include"../Symbol/SymbolTable.h"
#include"../Symbol/Symbol.h"
#include"../Error/ErrorToken.h"
class Node {
private:
    int pos;
    std::vector<Node> children;
    Node *parent;
    Token leafToken; //if is leaf
    GrammarType grammarType;  //non-leaf
    static std::vector<ErrorToken> errorList;
    static std::map<int, SymbolTable> symbolTableList;
    static int tableIdTop;
public:
    void addChild(const Node& childNode);
    void addLeaf(Token token,int pos);
    Node(Node *parent,GrammarType grammarType,int pos);
    Node(Node *parent,Token leafToken,int pos);
    Node *getParent();
    void setParent(Node* parent);
    std::vector<Node> getChildren();
    bool isLeaf();
    Token getToken();
    std::string getLeafTokenName();
    int getPos();
    int getLine();
    void output();
    void fileWrite(std::vector<std::string>& a);
    GrammarType getGrammarType();
    void buildSymbolTable(int curScope,int preScope);

    void buildConstDecl(int curScope,int preScope);
    void buildConstDef(int curScope,int preScope);
    void buildConstInitial(int curScope,int preScope);
    void buildVarDecl(int curScope,int preScope);
    void buildVarDef(int curScope,int preScope);
    void buildInitVal(int curScope,int preScope);
    void buildFuncDef(int curScope,int preScope);
    void buildMainFuncDef(int curScope,int preScope);
    void buildFuncFParams(int curScope,int preScope);
    void buildFuncFParam(int curScope,int preScope);
    void buildConstExp(int curScope,int preScope);
    void buildExp(int curScope,int preScope);
    void buildBlock(int curScope,int preScope,bool newTable);
    void buildBlockItem(int curScope,int preScope);
    void buildDecl(int curScope,int preScope);
    void buildStmt(int curScope,int preScope);
};


#endif //COMPILER_NODE_H

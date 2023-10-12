//
// Created by me on 2023/9/26.
//

#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H
#include<vector>
#include<iostream>
#include"Token.h"
#include"GrammarType.h"
#include"SymbolTable.h"
#include"Symbol.h"
#include"ErrorToken.h"
class Node {
private:
    int pos;
    std::vector<Node> children;
    Node *parent;
    Token leafToken; //if is leaf
    GrammarType grammarType;  //non-leaf
    std::vector<ErrorToken> errorList;
public:
    void addChild(Node childNode);
    void addLeaf(Token token,int pos);
    Node(Node *parent,GrammarType grammarType,int pos);
    Node(Node *parent,Token leafToken,int pos);
    Node *getParent();
    void setParent(Node* parent);
    std::vector<Node> getChildren();
    bool isLeaf();
    Token getToken();
    int getPos();
    int getLine();
    void output();
    void fileWrite(std::vector<std::string>& a);
    GrammarType getGrammarType();
    void buildSymbolTable(std::map<int,SymbolTable>& symbolTableList,int curScope,int preScope);

    void buildConstDecl(std::map<int,SymbolTable>& symbolTableList,int curScope,int preScope);
    void buildConstDef(std::map<int,SymbolTable>& symbolTableList,int curScope,int preScope);
    void buildConstInitial(std::map<int,SymbolTable>& symbolTableList,int curScope,int preScope);
    void buildVarDecl(std::map<int,SymbolTable>& symbolTableList,int curScope,int preScope);
    void buildVarDef(std::map<int,SymbolTable>& symbolTableList,int curScope,int preScope);
};


#endif //COMPILER_NODE_H

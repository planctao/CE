//
// Created by me on 2023/9/26.
//

#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H
#include<vector>
#include<iostream>
#include<map>
#include <utility>
#include<algorithm>
#include<fstream>
#include"Token.h"
#include"../Util/GrammarType.h"
#include"../Symbol/SymbolTable.h"
#include"../Symbol/Symbol.h"
#include"../Error/ErrorToken.h"
#include"../Util/utils.h"
#define GLOBAL -1
#define MAIN_FUNC 0
#define INT_FUNC 1
#define VOID_FUNC 2
#define NODE_DEBUG 1
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
    static int scopeType;
    static int loopTime;
public:
    void addChild(const Node& childNode);
    void addLeaf(Token token,int pos);
    void addLeaf(Token token,int pos,int vector_pos);
    void addLeaf(std::string conName,int line,int in_pos,int vector_pos);
    Node(Node *parent,GrammarType grammarType,int pos);
    Node(Node *parent,Token leafToken,int pos);
    Node *getParent();
    void setParent(Node* parent);
    std::vector<Node>getChildren();
    bool isLeaf();
    Token getToken();
    std::string getLeafTokenName();
    TokenType getLeafTokenType();
    int getPos();
    int getLine();
    void output();
    void fileWrite(std::vector<std::string>& a);
    GrammarType getGrammarType();
    static bool findIdent(std::string identName, int curScope,bool isVar);
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
    std::vector<int> buildConstExp(int curScope,int preScope);
    std::vector<int> buildExp(int curScope,int preScope);
    void buildBlock(int curScope,int preScope);
    void buildBlockItem(int curScope,int preScope);
    void buildDecl(int curScope,int preScope);
    void buildStmt(int curScope,int preScope);
    std::vector<int> buildLVal(int curScope,int preScope);
    void buildForStmt(int curScope,int preScope);
    void buildCond(int curScope,int preScope);
    std::vector<int> buildAddExp(int curScope,int preScope);
    std::vector<int> buildLOrExp(int curScope,int preScope);
    std::vector<int> buildLAndExp(int curScope,int preScope);
    std::vector<int> buildEqExp(int curScope,int preScope);
    std::vector<int> buildRelExp(int curScope,int preScope);
    std::vector<int> buildUnaryExp(int curScope,int preScope);
    std::vector<int> buildFuncRParams(int curScope,int preScope);
    std::vector<int> buildPrimaryExp(int curScope,int preScope);
    std::vector<int> buildMulExp(int curScope,int preScope);

    void error_output();
};


#endif //COMPILER_NODE_H

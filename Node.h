//
// Created by me on 2023/9/26.
//

#ifndef COMPILER_NODE_H
#define COMPILER_NODE_H
#include<vector>
#include<iostream>
#include"Token.h"
#include"GrammarType.h"
class Node {
private:
    std::vector<Node> children;
    Node *parent;
    Token leafToken; //if is leaf
    GrammarType grammarType;  //non-leaf
public:
    void addChild(Node childNode);
    void addLeaf(Token token);
    Node(Node *parent,GrammarType grammarType);
    Node(Node *parent,Token leafToken);
    Node *getParent();
    void setParent(Node* parent);
    std::vector<Node> getChildren();
    bool isLeaf();
    Token getToken();
    void output();
    void fileWrite(std::vector<std::string>& a);
};


#endif //COMPILER_NODE_H

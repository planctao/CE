//
// Created by me on 2023/9/26.
//

#include "Node.h"
#include "Token.h"
Node* Node::getParent() {
    return this->parent;
}

std::vector<Node> Node::getChildren() {
    return this->children;
}

void Node::setParent(Node *parent) {
    this->parent = parent;
}

Node::Node(Node *parent,GrammarType grammarType) : leafToken(TokenType::NullType,-1,"NullType"){
    this->parent = parent;
    this->grammarType = grammarType;
}

Node::Node(Node *parent,Token leafToken) : leafToken(leafToken){
    this->parent = parent;
    this->grammarType = GrammarType::NullType;
}

void Node::addChild(Node childNode) {
    this->children.push_back(childNode);
}

void Node::addLeaf(Token token) {
    Node tmpNode = Node(this,std::move(token));
    this->children.push_back(tmpNode);
}

bool Node::isLeaf() {
    return this->grammarType == GrammarType::NullType;
}

Token Node::getToken() {
    return this->leafToken;
}

GrammarType Node::getGrammarType() {
    return this->grammarType;
}

void Node::output() {
    for(Node tmpNode : this->children) {
        if(tmpNode.isLeaf()) {
            tmpNode.getToken().output();
        }
        else {
            tmpNode.output();
        }
    }
    std::string  grammarStr = grammarStrings[static_cast<int>(this->grammarType)];
    if (grammarStr != "BType" && grammarStr != "Decl" && grammarStr != "BlockItem" && grammarStr != "formatString") {
        std::cout << "<" << grammarStr << ">" << '\n';
    }

}

void Node::fileWrite(std::vector<std::string>& a) {
    for(Node tmpNode : this->children) {
        if(tmpNode.isLeaf()) {
            tmpNode.getToken().writeFile(a);
        }
        else {
            tmpNode.fileWrite(a);
        }
    }
    std::string  grammarStr = grammarStrings[static_cast<int>(this->grammarType)];
    if (grammarStr != "BType" && grammarStr != "Decl" && grammarStr != "BlockItem" && grammarStr != "formatString") {
        grammarStr = "<" + grammarStr + ">";
        a.emplace_back(grammarStr);
    }

}



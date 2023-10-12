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

Node::Node(Node *parent,GrammarType grammarType,int pos) : leafToken(TokenType::NullType,-1,"NullType"){
    this->parent = parent;
    this->grammarType = grammarType;
    this->pos = pos;
}

Node::Node(Node *parent,Token leafToken,int pos) : leafToken(leafToken){
    this->parent = parent;
    this->grammarType = GrammarType::NullType;
    this->pos = pos;
}

void Node::addChild(Node childNode) {
    this->children.push_back(childNode);
}

void Node::addLeaf(Token token,int pos) {
    Node tmpNode = Node(this,std::move(token),pos);
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

int Node::getPos() {
    return this->pos;
}

int Node::getLine() {
    return this->leafToken.getLine();
}

void Node::buildSymbolTable(std::map<int,SymbolTable> &symbolTableList,int curScope,int preScope) {
    if (this->grammarType == GrammarType::CompUnit) {
        SymbolTable tmpST(0,-1);
        symbolTableList.insert(std::pair<int,SymbolTable>(0,tmpST));
        for (Node& tmpNode : this->children) {
            tmpNode.buildSymbolTable(symbolTableList,0,-1);
        }
    }
    else if (this->grammarType == GrammarType::Decl) {
        for (Node& tmpNode : this->children) {
            tmpNode.buildSymbolTable(symbolTableList,curScope,preScope);
        }
    }
    else if (this->grammarType == GrammarType::ConstDecl) {
        buildConstDecl(symbolTableList,curScope,preScope);
    }
    else if (this->grammarType == GrammarType::ConstDef) {
        buildConstDef(symbolTableList,curScope,preScope);
    }
    else if (this->grammarType == GrammarType::ConstInitVal) {
        buildConstInitial(symbolTableList,curScope,preScope);
    }
    else if (this->grammarType == GrammarType::VarDecl) {
        buildVarDecl(symbolTableList,curScope,preScope);
    }
    else if (this->grammarType == GrammarType::VarDef) {
        buildVarDef(symbolTableList,curScope,preScope);
    }
}

void Node::buildConstDecl(std::map<int, SymbolTable> &symbolTableList, int curScope, int preScope) {
    auto iter = this->children.rbegin();
    /*先找到错误*/
    if ((*iter).getToken().getValue() != ";") {
        ErrorToken tmpErrorToken('i',(*iter).getToken().getLine(),(*iter).getPos());
        this->errorList.emplace_back(tmpErrorToken);
    }
    /*递归建表*/
    for (auto& tmpNode:this->children) {
        if (tmpNode.getGrammarType() == GrammarType::ConstDef) {
            tmpNode.buildSymbolTable(symbolTableList,curScope,preScope);
        }
    }
}

void Node::buildConstDef(std::map<int, SymbolTable> &symbolTableList, int curScope, int preScope) {
    /*处理有括号 type == k*/
    if(this->getChildren()[1].getToken().getValue() == "[") {
        int start = 1;
        while(start < this->getChildren().size() && this->getChildren()[start].getToken().getValue() == "[") {
            start += 2;
            if (start < this->getChildren().size() && this->getChildren()[start].getToken().getValue() != "]") {
                ErrorToken tmpErrorToken('k',this->getChildren()[start-1].getToken().getLine(),this->getChildren()[start].getPos());
                this->errorList.emplace_back(tmpErrorToken);
            }
            start++;
        }
    }
    /*重命名 type== b*/
    std::string tmpName = this->getChildren()[0].getToken().getValue();
    auto tmpST = symbolTableList[curScope];
    for (auto tmpSymbol : tmpST.getDirectory()) {
        if (tmpSymbol.second.getTokenValue() == tmpName) {
            ErrorToken tmpErrorToken('b',this->getChildren()[0].getToken().getLine(),this->getChildren()[0].getPos());
            this->errorList.emplace_back(tmpErrorToken);
            break;
        }
    }
    /*递归查询建表*/
    if (this->getChildren()[1].getToken().getValue() == "[") {
        int start = 1;
        while(start < this->getChildren().size() && this->getChildren()[start].getToken().getValue() == "[") {
            start++;
            auto& tmpConstExpNode = this->getChildren()[start];
            tmpConstExpNode.buildSymbolTable(symbolTableList,curScope,preScope);
        }
    }
    auto iter = this->getChildren().rbegin();
    if ((*iter).getGrammarType() == GrammarType::ConstInitVal) {
        (*iter).buildSymbolTable(symbolTableList,curScope,preScope);
    }
}

void Node::buildConstInitial(std::map<int, SymbolTable> &symbolTableList, int curScope, int preScope) {
    for (Node& tmpNode : this->getChildren()) {
        if (tmpNode.getGrammarType() == GrammarType::ConstExp || tmpNode.getGrammarType() == GrammarType::ConstInitVal) {
            tmpNode.buildConstDecl(symbolTableList,curScope,preScope);
        }
    }
}

void Node::buildVarDecl(std::map<int, SymbolTable> &symbolTableList, int curScope, int preScope) {
    auto iter = this->children.rbegin();
    /*先找到错误*/
    if ((*iter).getToken().getValue() != ";") {
        ErrorToken tmpErrorToken('i',(*iter).getToken().getLine(),(*iter).getPos());
        this->errorList.emplace_back(tmpErrorToken);
    }
    for (Node &tmpNode : this->children) {
        if (tmpNode.getGrammarType() == GrammarType::VarDef) {
            tmpNode.buildSymbolTable(symbolTableList,curScope,preScope);
        }
    }
}

void Node::buildVarDef(std::map<int, SymbolTable> &symbolTableList, int curScope, int preScope) {
    /*错误处理 b*/
    auto tmpST = symbolTableList[curScope];
    std::string indentName = this->getChildren()[0].getToken().getValue();
    for (auto tmpSymbol : tmpST.getDirectory()) {
        if (tmpSymbol.second.getTokenValue() == indentName) {
            ErrorToken tmpErrorToken('b',this->getChildren()[0].getToken().getLine(),this->getPos());
            this->errorList.emplace_back(tmpErrorToken);
        }
    }
    /*错误处理 k*/
    if (this->children.size() > 1 && this->children[1].getToken().getValue() == "[") {
        int start = 1;
        while(this->children[start].getToken().getValue() == "[") {
            start += 2;
            if (this->children[start].getToken().getValue() != "]") {
                ErrorToken tmpErrorToken('k',this->children[start-1].getLine(),this->children[start-1].getPos());
                this->errorList.emplace_back(tmpErrorToken);
            }
            start += 1;
        }
    }
}

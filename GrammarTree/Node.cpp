//
// Created by me on 2023/9/26.
//

#include "Node.h"
#include<cstdio>
int Node::tableIdTop = 0;
std::vector<ErrorToken> Node::errorList;
std::map<int, SymbolTable> Node::symbolTableList;
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

Node::Node(Node *parent,Token leafToken,int pos) : leafToken(std::move(leafToken)){
    this->parent = parent;
    this->grammarType = GrammarType::NullType;
    this->pos = pos;
}

void Node::addChild(const Node& childNode) {
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

std::string Node::getLeafTokenName() {
    return this->leafToken.getTokenName();
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

void Node::buildSymbolTable(int curScope,int preScope) {

    if (this->grammarType == GrammarType::CompUnit) {
        SymbolTable tmpST(0,-1);
        Node::symbolTableList.insert(std::make_pair(0,tmpST));
        for (Node& tmpNode : this->children) {
            tmpNode.buildSymbolTable(0,-1);
        }
    }
    else if (this->grammarType == GrammarType::Decl) {
        for (Node& tmpNode : this->children) {
            tmpNode.buildSymbolTable(curScope,preScope);
        }
    }
    else if (this->grammarType == GrammarType::ConstDecl) {
        buildConstDecl(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::ConstDef) {
        buildConstDef(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::ConstInitVal) {
        buildConstInitial(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::VarDecl) {
        buildVarDecl(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::VarDef) {
        buildVarDef(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::InitVal) {
        buildInitVal(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::FuncDef) {
        buildFuncDef(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::MainFuncDef) {
        buildMainFuncDef(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::FuncFParams) {
        buildFuncFParams(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::FuncFParam) {
        buildFuncFParam(curScope,preScope);
    }
    else if (this->grammarType == GrammarType::Block) {

    }
}

void Node::buildConstDecl(int curScope, int preScope) {
    auto iter = this->children.rbegin();
    /*先找到错误*/
    if ((*iter).getLeafTokenName() != ";") {
        ErrorToken tmpErrorToken('i',(*iter).getToken().getLine());
        this->errorList.emplace_back(tmpErrorToken);
    }
    /*递归建表*/
    for (auto& tmpNode:this->children) {
        if (tmpNode.getGrammarType() == GrammarType::ConstDef) {
            tmpNode.buildConstDef(curScope,preScope);
        }
    }
}

//ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal  error ==  b k
void Node::buildConstDef(int curScope, int preScope) {
    bool flag_b = false,flag_k = false; //表示是否出现b或者k错误
    /*处理有括号 type == k*/
    if(this->getChildren()[1].getLeafTokenName() == "[") {
        int start = 1;
        while(start < this->getChildren().size() && this->getChildren()[start].getLeafTokenName() == "[") {
            start += 2;
            if (start < this->getChildren().size() && this->getChildren()[start].getLeafTokenName() != "]") {
                ErrorToken tmpErrorToken('k',this->getChildren()[start-1].getToken().getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                flag_k = true;
            }
            start++;
        }
    }
    /*重命名 type== b*/
    std::string tmpName = this->getChildren()[0].getLeafTokenName();
    auto tmpST = Node::symbolTableList[curScope];
    for (auto tmpSymbol : tmpST.getDirectory()) {
        if (tmpSymbol.second.getTokenValue() == tmpName) {
            ErrorToken tmpErrorToken('b',this->getChildren()[0].getToken().getLine());
            Node::errorList.emplace_back(tmpErrorToken);
            flag_b = true;
            break;
        }
    }

    /*放入表中ident*/
    if (!flag_b) { //如果没出现b：重命名错误,就要放进去
        int dim = 0;//获得这个对象的类型(变量，一维数组，二维数组)
        for (auto tmpChild : this->children) {
            if (tmpChild.getLeafTokenName() == "[") {
                dim++;
            }
        }
        std::string identName = this->children[0].getLeafTokenName();
        int identPos = this->children[0].getPos();
        if (dim ==  0) {
            Symbol tmpSb(identPos,curScope,identName,VARIABLE_TYPE,CONST_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
        else if (dim == 1) {
            Symbol tmpSb(identPos,curScope,identName,ARRAY_1_TYPE,CONST_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
        else if (dim == 2) {
            Symbol tmpSb(identPos,curScope,identName,ARRAY_2_TYPE,CONST_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
    }

    /*递归查询继续建表*/
    if (this->getChildren()[1].getLeafTokenName() == "[") {
        int start = 1;
        while(start < this->getChildren().size() && this->getChildren()[start].getLeafTokenName() == "[") {
            start++;
            auto& tmpConstExpNode = this->getChildren()[start];
            tmpConstExpNode.buildConstExp(curScope,preScope);
        }
    }
    auto iter = this->getChildren().rbegin();
    if ((*iter).getGrammarType() == GrammarType::ConstInitVal) {
        (*iter).buildConstInitial(curScope,preScope);
    }
}

void Node::buildConstInitial(int curScope, int preScope) {
    for (Node& tmpNode : this->getChildren()) {
        if (tmpNode.getGrammarType() == GrammarType::ConstExp) {
            tmpNode.buildConstExp(curScope,preScope);
        }
        if (tmpNode.getGrammarType() == GrammarType::ConstInitVal) {
            tmpNode.buildInitVal(curScope,preScope);
        }
    }
}

void Node::buildVarDecl(int curScope, int preScope) {
    auto iter = this->children.rbegin();
    /*先找到错误*/
    if ((*iter).getLeafTokenName() != ";") {
        ErrorToken tmpErrorToken('i',(*iter).getToken().getLine());
        Node::errorList.emplace_back(tmpErrorToken);
    }
    for (Node &tmpNode : this->children) {
        if (tmpNode.getGrammarType() == GrammarType::VarDef) {
            tmpNode.buildVarDef(curScope,preScope);
        }
    }
}

/*VarDef → Ident { '[' ConstExp ']' } | Ident { '[' ConstExp ']' } '=' InitVal*/
void Node::buildVarDef(int curScope, int preScope) {
    bool flag_b = false, flag_k = false;
    /*错误处理 b*/
    auto tmpST = Node::symbolTableList[curScope];
    std::string indentName = this->getChildren()[0].getLeafTokenName();
    for (auto tmpSymbol : tmpST.getDirectory()) {
        if (tmpSymbol.second.getTokenValue() == indentName) {
            ErrorToken tmpErrorToken('b',this->getChildren()[0].getToken().getLine());
            Node::errorList.emplace_back(tmpErrorToken);
            flag_b = true;
        }
    }
    /*错误处理 k*/
    if (this->children.size() > 1 && this->children[1].getLeafTokenName() == "[") {
        int start = 1;
        while(this->children[start].getLeafTokenName() == "[") {
            start += 2;
            if (this->children[start].getLeafTokenName() != "]") {
                ErrorToken tmpErrorToken('k',this->children[start-1].getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                flag_k = true;
            }
            start += 1;
        }
    }

    /*如果没有重命名的错误，将这个变量放入到符号表中*/
    if (!flag_b) {
        int dim = 0;
        for (auto child : this->children) {
            if (child.getLeafTokenName() == "[") {
                dim++;
            }
        }
        if (dim == 0) {
            Symbol tmpSb(this->children[0].getPos(),curScope,this->children[0].getLeafTokenName(),VARIABLE_TYPE,VAR_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
        else if (dim == 1) {
            Symbol tmpSb(this->children[0].getPos(),curScope,this->children[0].getLeafTokenName(),ARRAY_1_TYPE,VAR_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
        else if (dim == 2) {
            Symbol tmpSb(this->children[0].getPos(),curScope,this->children[0].getLeafTokenName(),ARRAY_2_TYPE,VAR_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
    }

    /*继续递归建表*/
    for (Node &tmpChild : this->children) {
        if (tmpChild.getGrammarType() == GrammarType::ConstExp) {
            tmpChild.buildConstExp(curScope,preScope);
        }
        if (tmpChild.getGrammarType() == GrammarType::InitVal) {
            tmpChild.buildInitVal(curScope,preScope);
        }
    }
}

void Node::buildInitVal(int curScope, int preScope) {
    for (Node& tmpNode : this->getChildren()) {
        if (tmpNode.getGrammarType() == GrammarType::InitVal) {
            tmpNode.buildInitVal(curScope,preScope);
        }
        if (tmpNode.getGrammarType() == GrammarType::Exp) {
            tmpNode.buildExp(curScope,preScope);
        }
    }
}

void Node::buildFuncDef(int curScope, int preScope) {
    /*错误处理 重命名 type == b*/
    std::string identName = this->children[1].getLeafTokenName();
    SymbolTable tmpST = Node::symbolTableList[curScope];
    for (auto tmpSymbol : tmpST.getDirectory()) {
        if (tmpSymbol.second.getTokenValue() == identName) {
            ErrorToken tmpErrorToken('b',this->children[1].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }
    /*有返回值的是否有return type==g*/
    if (this->children[0].getLeafTokenName() == "int") {
        Node BlockNode = this->children.back();
        auto iter = BlockNode.getChildren().rbegin();
        iter++;
        if (!(iter->getChildren().size() >= 2 && iter->getChildren()[1].getGrammarType() == GrammarType::Exp)) {
            int errorLine = BlockNode.getChildren().rbegin()->getLine();
            ErrorToken tmpErrorToken('g',errorLine);
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }
    /*错误处理 小括号确实 type==j*/
    if (this->children[3].getGrammarType() == GrammarType::FuncFParams) {
        if (this->children[4].getLeafTokenName() != ")") {
            ErrorToken tmpErrorToken('j',this->children[3].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
        this->children[3].buildFuncFParams(curScope,preScope);
    }
    else {
        if (this->children[3].getLeafTokenName() != ")") {
            ErrorToken tmpErrorToken('j',this->children[2].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }
    /*建新的符号表*/
    std::vector<Symbol> funcParams;//函数传参要进入到Block里面来进行域的加入符号表
    if (this->children[3].getGrammarType() == GrammarType::FuncFParams) {
        for (auto child : this->children[3].getChildren()) {
            if (child.getGrammarType() == GrammarType::FuncFParam) {
                int dim = 0;
                for (auto para : child.getChildren()) {
                    if (para.getLeafTokenName() == "[") dim++;
                }
                if (dim == 0) {
                    Symbol paramSb(child.getPos(),Node::tableIdTop,child.getLeafTokenName(),VARIABLE_TYPE,VAR_KIND);
                    funcParams.emplace_back(paramSb);
                }
                else if (dim == 1) {
                    Symbol paramSb(child.getPos(),Node::tableIdTop,child.getLeafTokenName(),ARRAY_1_TYPE,VAR_KIND);
                    funcParams.emplace_back(paramSb);
                }
                else if (dim == 2) {
                    Symbol paramSb(child.getPos(),Node::tableIdTop,child.getLeafTokenName(),ARRAY_2_TYPE,VAR_KIND);
                    funcParams.emplace_back(paramSb);
                }
            }
        }
    }

    /*继续递归建表*/
    auto iter = this->children.rbegin();
    iter->buildBlock(curScope,preScope);
}

void Node::buildMainFuncDef(int curScope, int preScope) {
    /*错误处理 int返回值 type==g*/
    Node blockNode = this->children.back();
    auto iter = blockNode.getChildren().rbegin();
    iter++;
    if (!(iter->getChildren().size() >= 2 && iter->getChildren()[1].getGrammarType() == GrammarType::Exp)) {
        int errorLine = blockNode.getChildren().rbegin()->getLine();
        ErrorToken tmpErrorToken('g',errorLine);
        Node::errorList.emplace_back(tmpErrorToken);
    }
    /*错误处理 有括号缺失 type==j*/
    if (this->children[3].getLeafTokenName() != ")") {
        ErrorToken tmpErrorToken('j',this->children[2].getLine());
        Node::errorList.emplace_back(tmpErrorToken);
    }
    /*main函数里面要建立一个新的符号表了*/
    int newScope = Node::tableIdTop;
    Node::tableIdTop++;
    SymbolTable newTable(newScope,curScope);
    Node::symbolTableList.insert(std::make_pair(newScope,newTable));
    Node& tmpBlockNode = this->children.back();
    tmpBlockNode.buildSymbolTable(newScope,curScope);
}

void Node::buildFuncFParams(int curScope, int preScope) {
    for (auto &tmpNode : this->children) {
        if (tmpNode.getGrammarType() == GrammarType::FuncFParam) {
            tmpNode.buildSymbolTable(curScope,preScope);
        }
    }
}

void Node::buildFuncFParam(int curScope, int preScope) {
    /*错误处理 名字重定义 type==b*/
    bool flag_b = false, flag_k = false;
    std::string identName = this->children[1].getLeafTokenName();
    for (auto tmpSymbol : Node::symbolTableList[curScope].getDirectory()) {
        if (identName == tmpSymbol.second.getTokenValue()) {
            ErrorToken tmpErrorToken('b',this->children[1].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
            flag_b = true;
        }
    }
    /*错误处理 中括号缺失 type==k*/
    if(this->children[2].getLeafTokenName() == "[") {
        if (this->children[3].getLeafTokenName() != "]") {
            ErrorToken tmpErrorToken('k',this->children[2].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
            flag_k = true;
            if (this->children[3].getLeafTokenName() == "[") {
                if (this->children[5].getLeafTokenName() != "]") {
                    ErrorToken tmpErrorToken1('k',this->children[4].getLine());
                    Node::errorList.emplace_back(tmpErrorToken1);
                }
            }
        }
        else {
            if (this->children[4].getLeafTokenName() == "[") {
                if (this->children[6].getLeafTokenName() != "]") {
                    flag_k = true;
                    ErrorToken tmpErrorToken1('k',this->children[5].getLine());
                    Node::errorList.emplace_back(tmpErrorToken1);
                }
            }
        }
    }
    /*放入符号表*/
    if (!flag_b) {
        int dim = 0;
        for (auto tmpChild : this->children) {
            if (tmpChild.getLeafTokenName() == "[") dim++;
        }
        if (dim == 0) {
            Symbol tmpSb(this->children[1].getPos(),curScope,identName,VARIABLE_TYPE,VAR_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
        else if (dim == 1) {
            Symbol tmpSb(this->children[1].getPos(),curScope,identName,ARRAY_1_TYPE,VAR_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
        else if (dim == 2) {
            Symbol tmpSb(this->children[1].getPos(),curScope,identName,ARRAY_2_TYPE,VAR_KIND);
            Node::symbolTableList[curScope].addSymbol(tmpSb);
        }
    }

    /*递归遍历建表*/
    for(auto& tmpChild : this->children) {
        if (tmpChild.getGrammarType() == GrammarType::ConstExp) {
            tmpChild.buildSymbolTable(curScope,preScope);
        }
    }
}

void Node::buildBlock(int curScope, int preScope, bool newTable) {
    int nowScope = curScope;
    int upperScope = curScope;
    if (newTable) {
        SymbolTable newSt(Node::tableIdTop,curScope);
        Node::symbolTableList.insert(std::make_pair(Node::tableIdTop,newSt));
        nowScope = Node::tableIdTop;
        upperScope = curScope;
        Node::tableIdTop++;
    }
    for (auto& child : this->children) {
        if (child.getGrammarType() == GrammarType::BlockItem) {
            child.buildBlockItem(nowScope,upperScope);
        }
    }
}

void Node::buildBlockItem(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::Decl) {
        buildDecl(curScope,preScope);
    }
    else {
        buildStmt(curScope,preScope);
    }
}


void Node::buildStmt(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::LVal) { //LVal = Exp || LVal = getint()
        /*错误处理 常量值不能改变 type == h*/
        std::string LValName = this->children[0].getChildren()[0].getLeafTokenName();
        int tmpScope = curScope;
        while(tmpScope != -1) {
            for (Symbol symbol : )
        }
    }
}

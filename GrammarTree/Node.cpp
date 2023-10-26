//
// Created by me on 2023/9/26.
//

#include "Node.h"
#include<cstdio>
int Node::tableIdTop = 1;
int Node::loopTime = 0;
int Node::scopeType = GLOBAL;
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

void Node::addLeaf(Token token,int in_pos) {
    Node tmpNode = Node(this,std::move(token),in_pos);
    this->children.push_back(tmpNode);
}

void Node::addLeaf(Token token,int in_pos,int vector_pos) {
    Node tmpNode = Node(this,std::move(token),in_pos);
    this->children.insert(this->children.begin() + vector_pos,tmpNode);
}

void Node::addLeaf(std::string conName,int line,int in_pos,int vector_pos) {
    Token in_token(string2TokenType(conName),line,conName);
    Node tmpNode = Node(this,std::move(in_token),in_pos);
    this->children.insert(this->children.begin() + vector_pos,tmpNode);
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

TokenType Node::getLeafTokenType() {
    return this->leafToken.getTokenType();
}

GrammarType Node::getGrammarType() {
    return this->grammarType;
}

void Node::output() {
    std::string ans;
    std::fstream fout("output.txt");
    for(Node tmpNode : this->children) {
        if(tmpNode.isLeaf()) {
            ans += tmpNode.getToken().output();
        }
        else {
            tmpNode.output();
        }
    }
    std::string  grammarStr = grammarStrings[static_cast<int>(this->grammarType)];
    if (grammarStr != "BType" && grammarStr != "Decl" && grammarStr != "BlockItem" && grammarStr != "formatString") {
        fout << "<" << grammarStr << ">" << '\n';
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

bool Node::findIdent(std::string identName, int curScope, bool isVar) {
    bool flag = false;//是否找到符号表中的ident
    int tableID = curScope;
    while(tableID != -1) {
        for (auto tmpSb : Node::symbolTableList[tableID].getDirectory()) {
            if (isVar) {
                if (tmpSb.second.getSymbolType() != FUNC_TYPE && tmpSb.second.getTokenValue() == identName) {
                    return true;
                }
            }
            else {
                if (tmpSb.second.getSymbolType() == FUNC_TYPE && tmpSb.second.getTokenValue() == identName) {
                    return true;
                }
            }
        }
        tableID = Node::symbolTableList[tableID].getParentId();
    }
    return false;
}

/*CompUnit → {Decl} {FuncDef} MainFuncDef*/
void Node::buildSymbolTable(int curScope,int preScope) {
    SymbolTable tmpST(0,-1);
    Node::symbolTableList.insert(std::make_pair(0,tmpST));
    for (auto& tmpChild : this->children) {
        if (tmpChild.getGrammarType() == GrammarType::Decl) {
            tmpChild.buildDecl(0,-1);
        }
        else if (tmpChild.getGrammarType() == GrammarType::FuncDef) {
            tmpChild.buildFuncDef(0,-1);
        }
        else if (tmpChild.getGrammarType() == GrammarType::MainFuncDef) {
            tmpChild.buildMainFuncDef(0,-1);
        }
    }
}

/*Decl → ConstDecl | VarDecl*/
void Node::buildDecl(int curScope, int preScope) {
    for (Node& tmpNode : this->children) {
        if (tmpNode.getGrammarType() == GrammarType::ConstDecl) {
            tmpNode.buildConstDecl(curScope,preScope);
        }
        else if (tmpNode.getGrammarType() == GrammarType::VarDecl) {
            tmpNode.buildVarDecl(curScope,preScope);
        }
    }
}

/*ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';'*/
void Node::buildConstDecl(int curScope, int preScope) {
    auto iter = this->children.rbegin();
    /*先找到错误*/
    if (iter->getLeafTokenName() != ";") {
        Node tNode = *(iter->getChildren().rbegin());
        while(tNode.getGrammarType() != GrammarType::NullType) {
            tNode = *(tNode.getChildren().rbegin());
        }
        ErrorToken tmpErrorToken('i',tNode.getLine());
        Node::errorList.emplace_back(tmpErrorToken);
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
    if(this->children.size() > 1 && this->getChildren()[1].getLeafTokenName() == "[") {
        int start = 1;
        while(start < this->getChildren().size() && this->getChildren()[start].getLeafTokenName() == "[") {
            start += 2;
            if (start < this->getChildren().size() && this->getChildren()[start].getLeafTokenName() != "]") {
                Node tNode = this->getChildren()[start-1];
                while(tNode.getGrammarType() != GrammarType::NullType) {
                    tNode = *(tNode.getChildren().rbegin());
                }
                ErrorToken tmpErrorToken('k',tNode.getToken().getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                flag_k = true;
                start--;
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
//    for (int i = 0;i < this->children.size() && this->children[i].getLeafTokenName() != "=";i++) {
//        if (this->children[i].getGrammarType() == GrammarType::ConstExp) {
//            this->children[i].buildConstExp(curScope,preScope);
//        }
//    }
    if (this->children.size() > 1 && this->getChildren()[1].getLeafTokenName() == "[") {
        int start = 1;
        while(start < this->getChildren().size() && this->getChildren()[start].getLeafTokenName() == "[") {
            start++;
            auto& tmpConstExpNode = this->getChildren()[start];
            tmpConstExpNode.buildConstExp(curScope,preScope);
        }
    }
    if (this->children.back().getGrammarType() == GrammarType::ConstInitVal) {
        this->children.back().buildConstInitial(curScope,preScope);
    }
}


// ConstInitVal → ConstExp
//    | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'
void Node::buildConstInitial(int curScope, int preScope) {
    for (Node& tmpNode : this->children) {
        if (tmpNode.getGrammarType() == GrammarType::ConstExp) {
            tmpNode.buildConstExp(curScope,preScope);
        }
        else if (tmpNode.getGrammarType() == GrammarType::ConstInitVal) {
            tmpNode.buildConstInitial(curScope,preScope);
        }
    }
}

/* VarDecl → BType VarDef { ',' VarDef } ';'*/
void Node::buildVarDecl(int curScope, int preScope) {
    auto iter = this->children.rbegin();
    /*先找到错误*/
    if ((*iter).getLeafTokenName() != ";") {
        Node tNode = *(iter->children.rbegin());
        while(tNode.getGrammarType() != GrammarType::NullType) {
            tNode = *(tNode.getChildren().rbegin());
        }
        ErrorToken tmpErrorToken('i',tNode.getLine());
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
    /*错误处理 b  重命名*/
    auto tmpST = Node::symbolTableList[curScope];
    std::string indentName = this->getChildren()[0].getLeafTokenName();
    for (auto tmpSymbol : tmpST.getDirectory()) {
        if (tmpSymbol.second.getTokenValue() == indentName) {
            ErrorToken tmpErrorToken('b',this->getChildren()[0].getToken().getLine());
            Node::errorList.emplace_back(tmpErrorToken);
            flag_b = true;
        }
    }
    /*错误处理 k 右中括号*/
    if (this->children.size() > 1 && this->children[1].getLeafTokenName() == "[") {
        int start = 1;
        while(start < this->children.size() && this->children[start].getLeafTokenName() == "[") {
            start += 2;
            if (start >= this->children.size() || this->children[start].getLeafTokenName() != "]") {
                Node tNode = this->children[start - 1];
                while(tNode.getGrammarType() != GrammarType::NullType) {
                    tNode = *(tNode.getChildren().rbegin());
                }
                ErrorToken tmpErrorToken('k',tNode.getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                flag_k = true;
            }
            else {
                start += 1;
            }
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
    if (this->children[0].getChildren()[0].getLeafTokenName() == "int") {
        Node::scopeType = INT_FUNC;
    }
    else {
        Node::scopeType = VOID_FUNC;
    }
    /*错误处理 重命名 type == b*/
    std::string identName = this->children[1].getLeafTokenName();
    SymbolTable tmpST = Node::symbolTableList[curScope];
    for (const auto& tmpSymbol : tmpST.getDirectory()) {
        if (tmpSymbol.first == identName) {
            ErrorToken tmpErrorToken('b',this->children[1].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }
    /*有返回值的是否有return type==g*/
    if (this->children[0].getChildren()[0].getLeafTokenName() == "int") {
        Node BlockNode = this->children.back();
        auto iter = BlockNode.getChildren().rbegin();
        iter++;
        if (BlockNode.getChildren().size() <= 2) {
            int errorLine = BlockNode.getChildren().rbegin()->getLine();
            ErrorToken tmpErrorToken('g',errorLine);
            Node::errorList.emplace_back(tmpErrorToken);
        }
        else {
            Node blockItemNode = BlockNode.getChildren()[BlockNode.getChildren().size()-2];//倒数第二个blockItem
            Node blockStmtNode = blockItemNode.getChildren()[0];//blockItem->Stmt
            if (blockStmtNode.getChildren().size() < 2 ||
                blockStmtNode.getChildren()[0].getLeafTokenName() != "return" ||
                blockStmtNode.getChildren()[1].getGrammarType() != GrammarType::Exp) {
                int errorLine = BlockNode.getChildren().rbegin()->getLine();
                ErrorToken tmpErrorToken('g',errorLine);
                Node::errorList.emplace_back(tmpErrorToken);
            }
        }
    }
    /*错误处理 小括号确实 type==j*/
    if (this->children.size() > 3 && this->children[3].getGrammarType() == GrammarType::FuncFParams) {
        if (this->children.size() <= 4 || this->children[4].getLeafTokenName() != ")") {
            Node tNode = this->children[3];
            while(tNode.getGrammarType() != GrammarType::NullType) {
                tNode = *(tNode.getChildren().rbegin());
            }
            ErrorToken tmpErrorToken('j',tNode.getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
        //this->children[3].buildFuncFParams(curScope,preScope);
    }
    else {
        if (this->children.size() <= 3 || this->children[3].getLeafTokenName() != ")") {
            Node tNode = this->children[2];
            while(tNode.getGrammarType() != GrammarType::NullType) {
                tNode = *(tNode.getChildren().rbegin());
            }
            ErrorToken tmpErrorToken('j',tNode.getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }

    /*将函数参数放入符号表中*/
    std::vector<int> params;
    if (this->children.size() > 3 && this->children[3].getGrammarType() == GrammarType::FuncFParams) {
        for (auto child : this->children[3].getChildren()) {
            if (child.getGrammarType() == GrammarType::FuncFParam) {
                int paraDim = 0;
                for (auto cChild : child.getChildren()) {
                    if (cChild.getLeafTokenName() == "[") {
                        paraDim++;
                    }
                }
                params.emplace_back(paraDim);
            }
        }
    }
    int tmpReturn = this->children[0].getChildren()[0].getLeafTokenName() == "int" ? INT_FUNC : VOID_FUNC;
    Symbol funcDefSymbol(this->children[1].getPos(),curScope,this->children[1].getLeafTokenName(),
            FUNC_TYPE,tmpReturn,params.size(),params);
    Node::symbolTableList[curScope].addSymbol(funcDefSymbol);
    /*建新的符号表*/
    SymbolTable newST(Node::tableIdTop,curScope);Node::tableIdTop++;
    Node::symbolTableList.insert(std::make_pair(newST.getId(),newST));

    /*继续递归建表*/
    for (auto child : this->children) {
        if (child.getGrammarType() == GrammarType::FuncFParams) {
            child.buildFuncFParams(newST.getId(),curScope);
        }
    }
    auto iter = this->children.rbegin();
    iter->buildBlock(newST.getId(),curScope);
    Node::scopeType = GLOBAL;
}

void Node::buildMainFuncDef(int curScope, int preScope) {
    Node::scopeType = MAIN_FUNC;
    /*错误处理 int返回值 type==g*/
    Node blockNode = this->children.back();//block
    auto iter = blockNode.getChildren().rbegin();
    iter++;
    Node tmpNode = blockNode.getChildren()[blockNode.getChildren().size() - 2];//倒数第二个，blockItem
    if (blockNode.getChildren().size() <= 2) {
        int errorLine = blockNode.getChildren().rbegin()->getLine();
        ErrorToken tmpErrorToken('g',errorLine);
        Node::errorList.emplace_back(tmpErrorToken);
    }
    else if (!((tmpNode.getChildren()[0]).getGrammarType() == GrammarType::Stmt &&
            (tmpNode.getChildren()[0]).getChildren()[0].getLeafTokenName() == "return")) {
        int errorLine = blockNode.getChildren().rbegin()->getLine();
        ErrorToken tmpErrorToken('g',errorLine);
        Node::errorList.emplace_back(tmpErrorToken);
    }

    /*错误处理 有括号缺失 type==j*/
    if (this->children.size() <= 3 || this->children[3].getLeafTokenName() != ")") {
        Node tNode = this->children[2];
        while(tNode.getGrammarType() != GrammarType::NullType) {
            tNode = *(tNode.getChildren().rbegin());
        }
        ErrorToken tmpErrorToken('j',tNode.getLine());
        Node::errorList.emplace_back(tmpErrorToken);
    }
    /*main函数里面要建立一个新的符号表了*/
    int newScope = Node::tableIdTop;
    Node::tableIdTop++;
    SymbolTable newTable(newScope,curScope);
    Node::symbolTableList.insert(std::make_pair(newScope,newTable));
    Node& tmpBlockNode = this->children[this->children.size() - 1];
    //Node& tmpBlockNode = (this->children).back();
    tmpBlockNode.buildBlock(newScope,curScope);
    Node::scopeType = GLOBAL;
}

void Node::buildFuncFParams(int curScope, int preScope) {
    for (auto &tmpNode : this->children) {
        if (tmpNode.getGrammarType() == GrammarType::FuncFParam) {
            tmpNode.buildFuncFParam(curScope,preScope);
        }
    }
}

/*FuncFParam → BType Ident ['[' ']' { '[' ConstExp ']' }]*/
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
    if (this->children.size() > 2 && this->children[2].getLeafTokenName() == "[") {
        int start  = 0;
        if (this->children.size() <= 3 || this->children.at(3).getLeafTokenName() != "]") {
            ErrorToken tmpErrorToken('k',this->children.at(2).getLine());
            Node::errorList.emplace_back(tmpErrorToken);
            start = 3;
        }
        else {
            start = 4;
        }
        while(start < this->children.size() && this->children.at(start).getLeafTokenName() == "[") {
            start += 2;
            if (start >= this->children.size() || this->children.at(start).getLeafTokenName() != "]") {
                Node tNode = this->children[start - 1];
                while(tNode.getGrammarType() != GrammarType::NullType) {
                    tNode = *(tNode.getChildren().rbegin());
                }
                ErrorToken tmpErrorToken('k',tNode.getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                start--;
            }
            start++;
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
            tmpChild.buildConstExp(curScope,preScope);
        }
    }
}

void Node::buildBlock(int curScope, int preScope) {
    int nowScope = curScope;
    int upperScope = preScope;
    for (auto& child : this->children) {
        if (child.getGrammarType() == GrammarType::BlockItem) {
            child.buildBlockItem(nowScope,upperScope);
        }
    }
}

void Node::buildBlockItem(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::Decl) {
        this->children[0].buildDecl(curScope,preScope);
    }
    else {
        this->children[0].buildStmt(curScope,preScope);
    }
}


void Node::buildStmt(int curScope, int preScope) {
    /*错误处理 formatString type== a*/
    if (this->children[0].getLeafTokenType() == TokenType::PRINTFTK) {
        std::string formatString = this->children[2].getLeafTokenName();
        for (int i = 1;i < formatString.size()-1;i++) {
            if (formatString[i] == '%') {
                if (i + 1 < formatString.size() && formatString[i + 1] == 'd') {
                    continue;
                }
                else {
                    ErrorToken tmpErrorToken('a',this->children[2].getLine());
                    Node::errorList.emplace_back(tmpErrorToken);
                    break;
                }
            }
            else if (formatString[i] == '\\') {
                if (i + 1 >= formatString.size() || (i + 1 < formatString.size() && formatString[i+1] != 'n')) {
                    ErrorToken tmpErrorToken('a',this->children[2].getLine());
                    Node::errorList.emplace_back(tmpErrorToken);
                    break;
                }
            }
            else if (!(formatString[i] == 32 || formatString[i] == 33 || (formatString[i]>= 40 && formatString[i] <= 126))) {
                ErrorToken tmpErrorToken('a',this->children[2].getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                break;
            }
        }
    }
    /*错误处理 缺少分号 type==i*/
    if (this->children[0].getGrammarType() == GrammarType::LVal ||
        this->children[0].getGrammarType() == GrammarType::Exp ||
        this->children[0].getLeafTokenType() == TokenType::BREAKTK ||
        this->children[0].getLeafTokenType() == TokenType::CONTINUETK ||
        this->children[0].getLeafTokenType() == TokenType::RETURNTK ||
        this->children[0].getLeafTokenType() == TokenType::PRINTFTK) {
        auto iter = this->children.rbegin();
        if (iter->getLeafTokenName() != ";") {
            Node tNode = *(iter);
            while(tNode.getGrammarType() != GrammarType::NullType) {
                tNode = *(tNode.getChildren().rbegin());
            }
            ErrorToken tmpErrorToken('i',tNode.getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }
    /*错误处理 缺失右小括号 type==j*/
    if (this->children[0].getLeafTokenType() == TokenType::IFTK) { //if
        if (this->children[3].getLeafTokenName() != ")") {
            Node tNode = this->children[2];
            while(tNode.getGrammarType() != GrammarType::NullType) {
                tNode = *(tNode.getChildren().rbegin());
            }
            ErrorToken tmpErrorToken('j',tNode.getLine());
            Node::errorList.emplace_back(tmpErrorToken);
            addLeaf(")",tNode.getLine(),-1,3);
        }
    }
    if (this->children[0].getGrammarType() == GrammarType::LVal) { //LVal = getint();
        for (int i = 0;i < this->children.size();i++) {
            if (this->children[i].getLeafTokenName() == "getint") {
                if (!(i + 2 < this->children.size() && this->children[i + 2].getLeafTokenName() == ")")) {
                    Node tNode = this->children[i+1];
                    while(tNode.getGrammarType() != GrammarType::NullType) {
                        tNode = *(tNode.getChildren().rbegin());
                    }
                    ErrorToken tmpErrorToken('j',tNode.getLine());
                    Node::errorList.emplace_back(tmpErrorToken);
                }
                break;
            }
        }
    }
    if (this->children[0].getLeafTokenType() == TokenType::PRINTFTK) {
        auto printIt = this->children.rbegin();
        if (printIt->getLeafTokenName() == ";") {
            printIt++;
        }
        if (printIt->getLeafTokenName() != ")") {
            Node tNode = *(printIt+1);
            while(tNode.getGrammarType() != GrammarType::NullType) {
                tNode = *(tNode.getChildren().rbegin());
            }
            ErrorToken tmpErrorToken('j',tNode.getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }

    /*错误处理 break和continue语句需要在for循环里面使用 type==m*/
    if (this->children[0].getLeafTokenType() == TokenType::CONTINUETK || this->children[0].getLeafTokenType() == TokenType::BREAKTK) {
        if (Node::loopTime <= 0) {
            ErrorToken tmpErrorToken('m',this->children[0].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }
    /*错误处理 void的函数应该返回一个 return ; type == f*/
    if (this->children[0].getLeafTokenType() == TokenType::RETURNTK) {
        if (Node::scopeType != MAIN_FUNC && Node::scopeType != INT_FUNC && this->children.size() > 1 && this->children.at(1).getGrammarType() == GrammarType::Exp) {
            ErrorToken tmpErrorToken('f',this->children[0].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }

    /*错误处理 printf里面参数个数匹配  type==l*/
    if (this->children[0].getLeafTokenType() == TokenType::PRINTFTK) {
        std::string formatString = this->children[2].getLeafTokenName();
        int str_pos = 0;
        int para_num = 0;
        str_pos = formatString.find("%d");
        while(str_pos != std::string::npos) {
            para_num++;
            str_pos = formatString.find("%d",str_pos + 2);
        }
        for (auto child : this->children) {
            if (child.getGrammarType() == GrammarType::Exp) {
                para_num--;
            }
        }
        if (para_num != 0) {
            ErrorToken tmpErrorToken('l',this->children[0].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }

    /*错误处理 const不能改 type==h*/
    if (this->children[0].getGrammarType() == GrammarType::LVal) {
        std::string lValName = this->children[0].getChildren()[0].getLeafTokenName();
        int tableId = curScope;
        bool findIdent = false;
        while(tableId != -1) {//递归查找，如果找到了，是const说明这个不对，停止查找；如果找到了，是var，也停止查找
            for (auto tmpSymbol : Node::symbolTableList[tableId].getDirectory()) {
                if (tmpSymbol.second.getTokenValue() == lValName &&
                    tmpSymbol.second.getKind() == CONST_KIND) {
                    ErrorToken tmpErrorToken('h',this->children[0].getChildren()[0].getLine());
                    Node::errorList.emplace_back(tmpErrorToken);
                    findIdent = true;
                    break;
                }
                else if (tmpSymbol.second.getTokenValue() == lValName &&
                        tmpSymbol.second.getKind() == VAR_KIND) {
                    findIdent = true;
                }
            }
            if (findIdent)break;
            tableId = Node::symbolTableList[tableId].getParentId();
        }
    }
    /*递归建表*/
    if (this->children[0].getGrammarType() == GrammarType::LVal) {
        this->children[0].buildLVal(curScope,preScope);
        if (this->children[2].getLeafTokenName() != "getint") {
            this->children[2].buildExp(curScope,preScope);
        }
    }
    else if (this->children[0].getGrammarType() == GrammarType::Block) {
        SymbolTable newTable(Node::tableIdTop,curScope);
        Node::tableIdTop++;
        Node::symbolTableList.insert(std::make_pair(newTable.getId(),newTable));
        this->children[0].buildBlock(newTable.getId(),curScope);
    }
    else if (this->children[0].getGrammarType() == GrammarType::Exp) {
        this->children[0].buildExp(curScope,preScope);
    }
    else if (this->children[0].getLeafTokenName() == "if") {
        this->children[2].buildCond(curScope,preScope);
        this->children[4].buildStmt(curScope,preScope);
        if (this->children.size() > 6 && this->children[5].getLeafTokenName() == "else") {
            this->children[6].buildStmt(curScope,preScope);
        }
    }
    else if (this->children[0].getLeafTokenName() == "return") {
        if (this->children.size() > 1 && this->children[1].getGrammarType() == GrammarType::Exp) {
            this->children[1].buildExp(curScope,preScope);
        }
    }
    else if (this->children[0].getLeafTokenName() == "printf") {
        for (auto& child : this->children) {
            if (child.getGrammarType() == GrammarType::Exp) {
                child.buildExp(curScope,preScope);
            }
        }
    }
    else if (this->children[0].getLeafTokenName() == "for") {
        for (int i = 1;this->children[i].getLeafTokenName() != ")";i++) {
            if (this->children[i].getGrammarType() == GrammarType::ForStmt) {
                this->children[i].buildForStmt(curScope,preScope);
            }
            else if (this->children[i].getGrammarType() == GrammarType::Cond) {
                this->children[i].buildCond(curScope,preScope);
            }
        }
        Node::loopTime++;
        auto iter = this->children.rbegin();
        iter->buildStmt(curScope,preScope);
        Node::loopTime--;
    }
}

/* ForStmt → LVal '=' Exp*/
void Node::buildForStmt(int curScope, int preScope) {
    /*错误处理 LVal如果是const type==h*/
    std::string lValName = this->children[0].getChildren()[0].getLeafTokenName();
    int tableId = curScope;
    bool findIdent = false;
    while(tableId != -1) {
        for (auto tmpSymbol : Node::symbolTableList[tableId].getDirectory()) {
            if (tmpSymbol.second.getTokenValue() == lValName &&
                tmpSymbol.second.getKind() == CONST_KIND) {
                ErrorToken tmpErrorToken('h',this->children[0].getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                findIdent = true;
                break;
            }
            else if (tmpSymbol.second.getTokenValue() == lValName &&
                    tmpSymbol.second.getKind() == VAR_KIND) {
                findIdent = true;
                break;
            }
        }
        if (findIdent) break;
        tableId = Node::symbolTableList[tableId].getParentId();
    }

    this->children[0].buildLVal(curScope,preScope);
    this->children[2].buildExp(curScope,preScope);
}

/* Exp → AddExp */
std::vector<int> Node::buildExp(int curScope, int preScope) {
    return this->children[0].buildAddExp(curScope,preScope);
}

/*Cond → LOrExp */
void Node::buildCond(int curScope, int preScope) {
    this->children[0].buildLOrExp(curScope,preScope);
}

/*LVal → Ident {'[' Exp ']'}*/
std::vector<int> Node::buildLVal(int curScope, int preScope) {
    /*错误处理 未定义名字 type==c*/
    std::string identName = this->children[0].getLeafTokenName();
    bool flag = findIdent(identName,curScope,true);
    if (!flag) {
        ErrorToken tmpErrorToken('c',this->children[0].getLine());
        Node::errorList.emplace_back(tmpErrorToken);
    }
    /*错误处理 缺少中括号 type==k*/
    if (this->children.size() > 1 && this->children[1].getLeafTokenName() == "[") {
        int start = 1;
        while(start < this->children.size() && this->children[start].getLeafTokenName() == "[") {
            start += 2;
            if (start >= this->children.size() || this->children[start].getLeafTokenName() != "]") {
                Node tNode = this->children[start-1];
                while(tNode.getGrammarType() != GrammarType::NullType) {
                    tNode = *(tNode.getChildren().rbegin());
                }
                ErrorToken tmpErrorToken('k',tNode.getLine());
                Node::errorList.emplace_back(tmpErrorToken);
                start--;
            }
            start++;
        }
    }

    /*递归建表*/
    for (auto& tmpChild : this->children) {
        if (tmpChild.getGrammarType() == GrammarType::Exp) {
            tmpChild.buildExp(curScope,preScope);
        }
    }
    int defineType = 0;
    if (flag) {
        int tableId = curScope;
        /*递归向上查询变量*/
        while(tableId != -1) {
            for (auto sb : Node::symbolTableList[tableId].getDirectory()) {
                if (sb.first == identName && sb.second.getKind() == VAR_KIND) {
                    defineType = sb.second.getSymbolType();
                    tableId = -1;
                    break;
                }
            }
            if (tableId == -1)break;
            tableId = Node::symbolTableList[tableId].getParentId();
        }
    }
    int useDim = 0;
    for (auto child : this->children) {
        if (child.getLeafTokenName() == "[") {
            useDim++;
        }
    }
    std::vector<int> retDim;
    retDim.emplace_back(defineType - useDim);
    return retDim;
}

/*UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')' // c d e j
        | UnaryOp UnaryExp*/
std::vector<int> Node::buildUnaryExp(int curScope, int preScope) {
    int paraNum = 0;
    /*错误处理 未定义 type==c*/
    if (this->children[0].getGrammarType() != GrammarType::PrimaryExp && this->children[0].getGrammarType() != GrammarType::UnaryOp) {
        //说明这是ident开头的
        std::string name = this->children[0].getLeafTokenName();
        bool flag = findIdent(name,curScope,false);
        if (!flag) {
            ErrorToken tmpErrorToken('c',this->children[0].getLine());
            Node::errorList.emplace_back(tmpErrorToken);
        }
    }
    /*错误处理 函数个数不匹配 type == d  错误处理 函数实参和形参类型不匹配 type==e*/
    if(this->children[0].getGrammarType() != GrammarType::PrimaryExp && this->children[0].getGrammarType() != GrammarType::UnaryOp) {
        std::string identName = this->children[0].getLeafTokenName();
        /*先获取到实参的个数*/
        for (auto child : this->children) {
            if (child.getGrammarType() == GrammarType::FuncRParams) {
                for (auto child_leaf : child.getChildren()) {
                    if (child_leaf.getGrammarType() == GrammarType::Exp) {
                        paraNum++;
                    }
                }
            }
        }
        for (auto tmpSymbol : Node::symbolTableList[0].getDirectory()) {
            if (tmpSymbol.second.getTokenValue() == identName && tmpSymbol.second.getSymbolType() == FUNC_TYPE) {//如果找到了某个函数名字
                if (tmpSymbol.second.getParaNum() != paraNum) {
                    ErrorToken tmpErrorToken('d',this->children[0].getLine());
                    Node::errorList.emplace_back(tmpErrorToken);
                }
                else{
                    std::vector<int> paramDimList;
                    if (this->children.size() > 2 && this->children[2].getGrammarType() == GrammarType::FuncRParams) {
                        paramDimList = this->children[2].buildFuncRParams(curScope,preScope);
                    }
                    bool same_flag = true;
                    for (int i = 0;i < paramDimList.size();i++) {
                        if (paramDimList[i] != tmpSymbol.second.getParaTypes()[i]) {
                            same_flag = false;
                            break;
                        }
                    }
                    if (!same_flag) {
                        ErrorToken tmpErrorToken1('e',this->children[0].getLine());
                        Node::errorList.emplace_back(tmpErrorToken1);
                    }
                }
            }
        }
    }
    /*错误处理 右小括号不存在 type==j*/
    if(this->children[0].getGrammarType() != GrammarType::PrimaryExp && this->children[0].getGrammarType() != GrammarType::UnaryOp) {
        for (int i = 0;i < this->children.size();i++) {
            if (this->children[i].getLeafTokenName() == "(") {
                if (i + 1 < this->children.size() && this->children[i + 1].getGrammarType() == GrammarType::FuncRParams) {
                    if ((i + 2 >= this->children.size()) || (i + 2 < this->children.size() && this->children[i + 2].getLeafTokenName() != ")")) {
                        Node tNode = this->children[i+1];
                        while(tNode.getGrammarType() != GrammarType::NullType) {
                            tNode = *(tNode.getChildren().rbegin());
                        }
                        ErrorToken tmpErrorToken('j',tNode.getLine());
                        Node::errorList.emplace_back(tmpErrorToken);
                    }
                }
                else {
                    if ((i + 1 >= this->children.size()) || (i + 1 < this->children.size() && this->children[i + 1].getLeafTokenName() != ")")) {
                        Node tNode = this->children[i];
                        while(tNode.getGrammarType() != GrammarType::NullType) {
                            tNode = *(tNode.getChildren().rbegin());
                        }
                        ErrorToken tmpErrorToken('j',tNode.getLine());
                        Node::errorList.emplace_back(tmpErrorToken);
                    }
                }
            }
        }
    }
    std::vector<int> retDim;
    if (this->children[0].getGrammarType() == GrammarType::PrimaryExp) {
        retDim = this->children[0].buildPrimaryExp(curScope,preScope);
    }
    else if (this->children[0].getGrammarType() == GrammarType::UnaryOp) {
        retDim = this->children[1].buildUnaryExp(curScope,preScope);
    }
    else {
        for (auto child : Node::symbolTableList[0].getDirectory()) {
            if (child.second.getTokenValue() == this->children[0].getLeafTokenName() &&
                child.second.getSymbolType() == FUNC_TYPE) {
                if (child.second.getReturnType() == INT_FUNC) {
                    retDim.emplace_back(0);
                }
                else {
                    retDim.emplace_back(-1);
                }
            }
        }
    }
    return retDim;
}

std::vector<int> Node::buildPrimaryExp(int curScope, int preScope) {
    std::vector<int> retDim;
    if (this->children[0].getLeafTokenName() == "(") {
        return this->children[1].buildExp(curScope,preScope);
    }
    else if (this->children[0].getGrammarType() == GrammarType::LVal) {
        return this->children[0].buildLVal(curScope,preScope);
    }
    else {
        retDim.push_back(0);
    }
    return retDim;
}

/*FuncRParams → Exp { ',' Exp }*/
std::vector<int> Node::buildFuncRParams(int curScope, int preScope) {
    std::vector<int> retDim;
    for (auto &child : this->children) {
        if (child.getGrammarType() == GrammarType::Exp) {
            std::vector<int> expRet = child.buildExp(curScope,preScope);
            retDim.insert(retDim.end(),expRet.begin(),expRet.end());
        }
    }
    return retDim;
}

/*MulExp → UnaryExp | MulExp ('*' | '/' | '%') UnaryExp */
std::vector<int> Node::buildMulExp(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::UnaryExp) {
        return this->children[0].buildUnaryExp(curScope,preScope);
    }
    else {
        this->children[0].buildMulExp(curScope,preScope);
        return this->children[2].buildUnaryExp(curScope,preScope);
    }
}

/*AddExp → MulExp | AddExp ('+' | '−') MulExp */
std::vector<int> Node::buildAddExp(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::MulExp) {
        return this->children[0].buildMulExp(curScope,preScope);
    }
    else {
        this->children[0].buildAddExp(curScope,preScope);
        return this->children[2].buildMulExp(curScope,preScope);
    }
}

/*RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp*/
std::vector<int> Node::buildRelExp(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::AddExp) {
        return this->children[0].buildAddExp(curScope,preScope);
    }
    else {
        this->children[0].buildRelExp(curScope,preScope);
        return this->children[2].buildAddExp(curScope,preScope);
    }
}

/*EqExp → RelExp | EqExp ('==' | '!=') RelExp*/
std::vector<int> Node::buildEqExp(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::RelExp) {
        return this->children[0].buildRelExp(curScope,preScope);
    }
    else {
        this->children[0].buildEqExp(curScope,preScope);
        return this->children[2].buildRelExp(curScope,preScope);
    }
}

/*LAndExp → EqExp | LAndExp '&&' EqExp*/
std::vector<int> Node::buildLAndExp(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::EqExp) {
        return this->children[0].buildEqExp(curScope,preScope);
    }
    else {
        this->children[0].buildLAndExp(curScope,preScope);
        return this->children[2].buildEqExp(curScope,preScope);
    }
}

/*LOrExp → LAndExp | LOrExp '||' LAndExp */
std::vector<int> Node::buildLOrExp(int curScope, int preScope) {
    if (this->children[0].getGrammarType() == GrammarType::LAndExp) {
        return this->children[0].buildLAndExp(curScope,preScope);
    }
    else {
        this->children[0].buildLOrExp(curScope,preScope);
        return this->children[2].buildLAndExp(curScope,preScope);
    }
}

void Node::error_output() {
    std::ofstream fileout("error.txt");

    sort(Node::errorList.begin(), Node::errorList.end(),
         [](ErrorToken a, ErrorToken b){
                 return a.getErrorTokenLine() < b.getErrorTokenLine();
         });

    for (auto i : Node::errorList) {
        fileout << i.getOutputString();
    }



}

/*ConstExp → AddExp*/
std::vector<int> Node::buildConstExp(int curScope, int preScope) {
    return this->children[0].buildAddExp(curScope,preScope);
}


//
// Created by me on 2023/11/9.
//

#include "FunctionBuilder.h"

FunctionBuilder::FunctionBuilder(SymbolTable *curTable, Node *curNode) {
    this->curNode = curNode;
    this->curTable = curTable;
}

void FunctionBuilder::gen() {
    if (isDecl((*this->curNode))) {
        genGlobalVar(this->curNode);
    }
    else if (isFuncDef((*this->curNode))) {
        genFunction(this->curNode);
    }
    else if (isMainFuncDef((*this->curNode))) {
        genFunction(this->curNode);
    }
    else {
        std::cout << "invalid input in FunctionBuilder::gen Line == 23\n";
    }
//    for (auto &child : this->curNode->getChildren()) {
//        if (isDecl(child)) {
//            genGlobalVar(&child);
//        }
//        else {
//            genFunction(&child);
//        }
//    }
}

/*Decl -> ConstDecl | VarDecl*/
void FunctionBuilder::genGlobalVar(Node *tmpNode) {
    Node childNode = tmpNode->getChildren()[0];//ConstDecl | VarDecl
    bool ifConst = childNode.getGrammarType() == GrammarType::ConstDecl;
    for (auto &child : childNode.getChildren()) {
        if(isConstDef(child) || isVarDef(child)) {
            GlobalVar *globalVar = new GlobalVar(ifConst,"",&child);
            globalVar->buildGlobalVar(this->curTable);
            globalVar->buildSymbol(this->curTable);
            this->globalVarList.emplace_back(globalVar);
        }
    }

}

void FunctionBuilder::genFunction(Node *tmpNode) {
    Function *tmpFunc = new Function(tmpNode,this->curTable);

    tmpFunc->buildFunction();
    this->functionList.emplace_back(tmpFunc);

}

std::vector<Function*> FunctionBuilder::getFunctionList() {
    return this->functionList;
}

std::vector<GlobalVar*> FunctionBuilder::getGlobalList() {
    return this->globalVarList;
}

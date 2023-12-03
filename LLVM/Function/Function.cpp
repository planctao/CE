//
// Created by me on 2023/10/31.
//

#include "Function.h"
#include "../Module/Module.h"
#include "../../Symbol/SymbolVar.h"
#include "../../Symbol/SymbolFunc.h"
#include "../Instruction/InstructionBuilder.h"
#include "../BasicBlock/BasicBlockBuilder.h"
#include <utility>

Function::Function(std::string tmpName, Node *curNode, SymbolTable *curTable) : IrValue(std::move(tmpName)){
    this->curTable = curTable;
    this->curNode = curNode;
    this->identCnt = new int(0);
    if (this->curNode->getChildren()[0].getLeafTokenName() == "int") {
        this->returnType = "i32";
    }
    else if (this->curNode->getChildren()[0].getGrammarType() == GrammarType::FuncType){
        if (this->curNode->getChildren()[0].getChildren()[0].getLeafTokenName() == "int") {
            this->returnType = "i32";
        }
        else if (this->curNode->getChildren()[0].getChildren()[0].getLeafTokenName() == "void"){
            this->returnType = "void";
        }
    }
}


Function::Function(Node *curNode, SymbolTable *curTable) : IrValue(""){
    this->curNode = curNode;
    this->curTable = curTable;
    if (this->curNode->getChildren()[0].getLeafTokenName() == "int") {
        this->returnType = "i32";
    }
    else if (this->curNode->getChildren()[0].getGrammarType() == GrammarType::FuncType){
        if (this->curNode->getChildren()[0].getChildren()[0].getLeafTokenName() == "int") {
            this->returnType = "i32";
        }
        else if (this->curNode->getChildren()[0].getChildren()[0].getLeafTokenName() == "void"){
            this->returnType = "void";
        }
    }
    this->identCnt = new int(0);
}

void Function::buildFunction() {
    this->name = "@" + this->curNode->getChildren()[1].getLeafTokenName();
    buildFuncSymbol();
    SymbolTable *newTable = new SymbolTable(this->curTable);
    /*把参数放进去*/
    if (this->curNode->getChildren().size() > 3 &&
        this->curNode->getChildren()[3].getGrammarType() == GrammarType::FuncFParams) {
        Node childNode = this->curNode->getChildren()[3];
        for (auto &tmpNode : childNode.getChildren()) {
            if (tmpNode.getGrammarType() == GrammarType::FuncFParam) {
                buildFuncParam(&tmpNode,newTable);
            }
        }
    }
    /*this->curNode->getChildren().back() = Block*/
    Node tmpNode = this->curNode->getChildren().back();
    BasicBlockBuilder *blockBuilder = new BasicBlockBuilder(newTable,this->curNode,this->identCnt);
    blockBuilder->buildBasicBlocks();
    std::vector<BasicBlock*> tarBlockList = blockBuilder->getBasicBlock();
    this->basicBlockList.insert(this->basicBlockList.end(),tarBlockList.begin(),tarBlockList.end());
}

/*FuncFParam → BType Ident ['[' ']' { '[' ConstExp ']' }]*/
void Function::buildFuncParam(Node *tmpNode,SymbolTable* newTable) {
    int dim = tmpNode->calDim();
    auto *tmpParam = new Param(tmpNode,newTable,this->identCnt);
    tmpParam->setName("%instr_" + std::to_string(getFuncCnt(this->identCnt)));
    if (dim == 0) {
        auto *tmpParamSb = new SymbolVar(tmpNode->getChildren()[1].getLeafTokenName(),tmpParam,this->curTable);
        newTable->addSymbol((Symbol*)tmpParamSb);
        this->paramList.emplace_back(tmpParam);
    }
    else if (dim == 1) {
        auto *tmpParamSb = new SymbolVar(tmpNode->getChildren()[1].getLeafTokenName(),tmpParam,this->curTable,-1);
        newTable->addSymbol((Symbol*)tmpParamSb);
        this->paramList.emplace_back(tmpParam);
    }
    else if (dim == 2) {
        int constExpValue = -999;
        for (auto child : tmpNode->getChildren()) {
            if (isConstExp(child)) {
                constExpValue = child.calConstExp(this->curTable);
            }
        }
        auto *tmpParamSb = new SymbolVar(tmpNode->getChildren()[1].getLeafTokenName(),tmpParam,this->curTable,-1,constExpValue);
        newTable->addSymbol((Symbol*)tmpParamSb);
        this->paramList.emplace_back(tmpParam);
    }
}

std::string Function::irOutput() {
    std::string ans;
    ans += "define dso_local " + this->returnType + " " + this->name + "(";
    for (int i = 0;i < this->paramList.size();i++) {
        ans += paramList[i]->irOutput();
        if(i < this->paramList.size() - 1) {
            ans += ", ";
        }
    }

    ans +=  ") {\n";
    for (auto & i : basicBlockList) {
        ans += i->irOutput() + "\n";
    }
    ans += "}\n";
    return ans;
}


void Function::buildFuncSymbol() {
    std::string funcName = this->name;
    funcName.erase(0,1);
    SymbolFunc* funcSb = new SymbolFunc(funcName,(IrValue*)this,this->curTable);
    for (auto &parm : this->paramList) {
        funcSb->addParamSymbol(parm->genParamSymbol());
    }
    this->curTable->addSymbol((Symbol*)funcSb);
}

std::string Function::getReturnType() {
    return this->returnType;
}

std::vector<Param *> Function::getParamList() {
    return this->paramList;
}

std::vector<BasicBlock *> Function::getBasicBlock() {
    return this->basicBlockList;
}



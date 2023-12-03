//
// Created by me on 2023/11/1.
//

#include "Param.h"
#include "../ValueType/PointerType.h"
#include "../ValueType/IntegerType.h"

#include <utility>



/* BType Ident ['[' ']' { '[' ConstExp ']' }]*/
void Param::buildParam() {
    std::string identName = this->curNode->getChildren()[1].getLeafTokenName();
    int dim = curNode->calDim();
    if (dim == 2) {
        for (auto & child : this->curNode->getChildren()) {
            if (isConstExp(child)) {
                this->lastDim = child.calConstExp(curTable);
            }
        }
    }
}

Symbol *Param::genParamSymbol() {
    SymbolVar *sb;
    if (this->dim == 0) {
        sb = new SymbolVar(this->name,(IrValue*)this,nullptr);
        return (Symbol*)sb;
    }
    else if (this->dim == 1) {
        sb = new SymbolVar(this->name,(IrValue*)this, nullptr,-1);
        return (Symbol*)sb;

    }
    else if (this->dim == 2){
        sb = new SymbolVar(this->name,(IrValue*)this,nullptr,-1,lastDim);
        return (Symbol*)sb;
    }
    else {
        std::cout << "genParamSymbol error\n";
        //invalid
    }
    return nullptr;
}

Param::Param(Node *curNode, SymbolTable* curTable,int *cnt) : IrValue(""){
    this->curTable = curTable;
    this->curNode = curNode;
    this->cnt = cnt;
    this->dim = curNode->calDim();
    if (this->dim == 0) {
        this->valueType = new IntegerType(32);
    }
    else if (this->dim == 1) {
        this->valueType = new PointerType(new IntegerType(32));
    }
    else if (this->dim == 2) {
        this->valueType = new PointerType(new IntegerType(32));
    } else {std::cout << "construct Param::Param error\n";}
    for (auto child : curNode->getChildren()) {
        if (isConstExp(child)) {
            this->lastDim = child.calConstExp(curTable);
        }
    }
}

std::string Param::irOutput() {
    std::string ans;

    if (this->dim == 0) {
        ans += "i32 " + this->name;
    }
    else if (this->dim == 1) {
        ans += "i32* " + this->name;
    }
    else if (this->dim == 2) {
        ans += "i32* " + this->name;
    }
    else {
        /*invalid;*/
    }
    return ans;
}

int Param::getDim() {
    return this->dim;
}


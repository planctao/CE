//
// Created by me on 2023/10/31.
//

#include "GlobalVar.h"

#include <utility>
#include "../ValueType/PointerType.h"
#include "../ValueType/ValueType.h"
#include "../ValueType/ArrayType.h"
#include "../ValueType/IntegerType.h"


//GlobalVar::GlobalVar(bool ifConst, int dim, std::string varName, int single_value) : IrValue(std::move(varName)){
//    this->curNode = nullptr;
//    this->dim = dim;
//    this->ifConst = ifConst;
//    std::vector<int> tmpValue = {single_value};
//    this->valueList.emplace_back(tmpValue);
//}
//
//GlobalVar::GlobalVar(bool ifConst, int dim, std::string varName,
//        int size, std::vector<int> valueList) : IrValue(std::move(varName)){
//    this->curNode = nullptr;
//    this->dim = dim;
//    this->ifConst = ifConst;
//    this->dimList.emplace_back(size);
//    this->valueList.emplace_back(valueList);
//}
//
//GlobalVar::GlobalVar(bool ifConst, int dim, std::string varName, std::vector<int> dimList,
//                     std::vector<std::vector<int>> valueList) : IrValue(std::move(varName)){
//    this->curNode = nullptr;
//    this->dim = dim;
//    this->ifConst = ifConst;
//    this->dimList = std::move(dimList);
//    this->valueList = std::move(valueList);
//}

GlobalVar::GlobalVar(bool ifConst,std::string varName,Node *curNode) : IrValue(std::move(varName)), curNode(curNode) {
    this->ifConst = ifConst;
    this->curNode = curNode;
    this->dim = -1;
}

/*ConstDef | VarDef*/
void GlobalVar::buildGlobalVar(SymbolTable* curTable) {
    this->dim = curNode->calDim();
    this->name = "@" + curNode->getChildren()[0].getLeafTokenName();
    /*this->dimList*/
    for (auto child : this->curNode->getChildren()) {
        if (child.getGrammarType() == GrammarType::ConstExp) {
            this->dimList.emplace_back(child.calConstExp(curTable));
        }
    }
    /*this->arrayList*/
    if (this->ifConst) {
        this->arrayList = this->curNode->getChildren().back().calConstInitVal(curTable);
        if (this->dim == 0) {
            this->valueType = new PointerType(new IntegerType(32));
        }
        else if (this->dim == 1 || this->dim == 2) {
            ArrayType *array = new ArrayType(this->arrayList.size(),new IntegerType(32));
            this->valueType = new PointerType(array);

        }else {std::cout << "error in buildGlobalVar\n";}
        return;
    }
    /*剩下的只有是VarDecl,如果他的最后一个是InitVal*/
    if(this->curNode->getChildren().back().getGrammarType() == GrammarType::InitVal) {
        this->arrayList = this->curNode->getChildren().back().calInitVal(curTable);
        if (this->dim == 0) {
            this->valueType = new PointerType(new IntegerType(32));
        }
        else if (this->dim == 1 || this->dim == 2) {
            ArrayType *array = new ArrayType(this->arrayList.size(),new IntegerType(32));
            this->valueType = new PointerType(array);

        }else {std::cout << "error in buildGlobalVar\n";}
        return ;
    }
    /*剩下的就是VarDecl,并且他的最后一个不是InitVal需要我们对其进行初始化，也就是全置零*/
    switch (this->dim) {
        case 0:{
            this->arrayList = {0};
            break;
        }
        case 1:{
            std::vector<int> tmpList(this->dimList[0],0);
            this->arrayList.insert(this->arrayList.end(),tmpList.begin(),tmpList.end());
            break;
        }
        case 2:{
            for (int i = 0;i < this->dimList[0];i++) {
                std::vector<int> tmpList(this->dimList[1],0);
                this->arrayList.insert(this->arrayList.end(),tmpList.begin(),tmpList.end());
            }
            break;
        }
        default:{
            std::cout << "invalid dimension in GlobalVar::buildGlobalVar Line 78\n";
        }
    }
    if (this->dim == 0) {
        this->valueType = new PointerType(new IntegerType(32));
    }
    else if (this->dim == 1 || this->dim == 2) {
        ArrayType *array = new ArrayType(this->arrayList.size(),new IntegerType(32));
        this->valueType = new PointerType(array);

    }else {std::cout << "error in buildGlobalVar\n";}

}

std::string GlobalVar::irOutput() {
    std::string ans;
    if (this->dim == 0) {
        ans += this->name + " = dso_local global " + "i32 " + std::to_string(this->arrayList[0]);
    }
    else if (this->dim == 1 || this->dim == 2) {
        bool all_zero = true;
        for (int i = 0;i < this->dimList[0];i++) {
            if (this->arrayList[i] != 0) {
                all_zero = false;
                break;
            }
        }
        if (all_zero) {
            ans += this->name + " = dso_local global " + "[" + std::to_string(this->arrayList.size()) + "x " + "i32] ";
            ans += "zeroinitializer";
            return ans;
        }
        ans += this->name + " = dso_local global " + "[" + std::to_string(this->arrayList.size()) + "x " + "i32] " + "[";
        for (int i = 0;i < this->arrayList.size();i++) {
            ans += "i32 " + std::to_string(this->arrayList[i]);
            if(i < this->arrayList.size() - 1) {
                ans += ", ";
            }
        }
        ans += "]";
    }
    else {
        /*TODO*/
    }
    return ans;
}

void GlobalVar::buildSymbol(SymbolTable* curTable) {
    if (this->ifConst) {
        SymbolCon *sb;
        if (this->dim == 0) {
            std::string sybName = this->name;
            sybName.erase(0,1);
            sb = new SymbolCon(sybName,this->arrayList[0],(IrValue*)this,curTable);
            curTable->addSymbol((Symbol*)sb);
        }
        else if (this->dim == 1) {
            std::string sybName = this->name;
            sybName.erase(0,1);
            sb = new SymbolCon(sybName,this->arrayList,(IrValue*)this,curTable);
            sb->setDimList(this->dimList[0]);
            curTable->addSymbol((Symbol*)sb);

        }
        else if (this->dim == 2) {
            std::string sybName = this->name;
            sybName.erase(0,1);
            std::vector<std::vector<int>> ans = {this->arrayList};
            sb = new SymbolCon(sybName,ans,(IrValue*)this,curTable);
            sb->setDimList(this->dimList);
            curTable->addSymbol((Symbol*)sb);
        }
        else {
            /*invalid*/
        }
    }
    else {
        SymbolVar* sb;
        if (this->dim == 0) {
            std::string sybName = this->name;
            sybName.erase(0,1);
            sb = new SymbolVar(sybName,(IrValue*)this,curTable);
            curTable->addSymbol((Symbol*)sb);
        }
        else if (this->dim == 1) {
            std::string sybName = this->name;
            sybName.erase(0,1);
            sb = new SymbolVar(sybName,(IrValue*)this,curTable,this->dimList[0]);
            curTable->addSymbol((Symbol*)sb);
        }
        else if (this->dim == 2) {
            std::string sybName = this->name;
            sybName.erase(0,1);
            sb = new SymbolVar(sybName,(IrValue*)this,curTable,this->dimList[0],this->dimList[1]);
            curTable->addSymbol((Symbol*)sb);
        }
        else {
            /*invalid*/
        }
    }
}

std::vector<int> GlobalVar::getArrayList() {
    return this->arrayList;
}

int GlobalVar::getDim() {
    return this->dim;
}



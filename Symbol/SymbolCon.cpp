//
// Created by me on 2023/11/8.
//

#include "SymbolCon.h"

#include <utility>

/*常量*/
SymbolCon::SymbolCon(std::string identName, int init, IrValue * value ,SymbolTable *curTable) {
    this->init = init;

    this->tokenName = std::move(identName);
    this->irValue = value;
    this->dim = 0;
    this->kind = CONST_KIND;
    this->type = VARIABLE_TYPE;
}

/*常量一维数组*/
SymbolCon::SymbolCon(std::string identName, std::vector<int> init, IrValue *value, SymbolTable *curTable) {
    this->init = 999999999;//init 没有意义随便设置的数值

    this->initArray1 = std::move(init);
    this->tokenName = std::move(identName);
    this->irValue = value;
    this->dim = 1;
    this->kind = CONST_KIND;
    this->type = ARRAY_1_TYPE;
}

/*常量二维数组*/
SymbolCon::SymbolCon(std::string identName, std::vector<std::vector<int>> init, IrValue *value, SymbolTable *curTable) {
    this->init = 999999999;//init 没有意义随便设置的数值

    this->initArray2 = std::move(init);
    this->tokenName = std::move(identName);
    this->irValue = value;
    this->dim = 2;
    this->kind = CONST_KIND;
    this->type = ARRAY_2_TYPE;
}

int SymbolCon::calInit() {
    return this->init;
}

int SymbolCon::calInit(int x) {
    return this->initArray1[x];
}

int SymbolCon::calInit(int x,int y) {
    return this->initArray2[x][y];
}

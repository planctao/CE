//
// Created by me on 2023/10/10.
//

#include "Symbol.h"

#include <utility>

Symbol::Symbol(int id,int tableId,std::string tokenValue,int type,int kind) {
    this->id = id;
    this->tableId = tableId;
    this->tokenName = std::move(tokenValue);
    this->type = type;
    this->kind = kind;
    /*no mean*/
    this->returnType = -1;
    this->paramNum = -1;
    this->curTable = nullptr;
    this->dim = -1;
    this->irValue = nullptr;
}

Symbol::Symbol(int id, int tableId, std::string tokenName, int type, int returnType, int paramNum,
               std::vector<int> paraTypes) {
    this->id = id;
    this->tableId = tableId;
    this->tokenName = std::move(tokenName);
    this->type = type;
    this->returnType = returnType;
    this->paramNum = paramNum;
    this->paraTypes = std::move(paraTypes);
    this->curTable = nullptr;
    this->dim = -1;
    this->curTable = nullptr;
    this->irValue = nullptr;
    this->kind = VAR_KIND;
}

Symbol::Symbol(int tableId,std::string tokenName,int type,int kind,
        int dim,std::vector<int> dimList,std::vector<std::vector<int>> valueList) {
    this->id = -1;
    this->tableId = tableId;
    this->tokenName = std::move(tokenName);
    this->type = type;
    this->kind = kind;
    this->dim = dim;
    this->dimList = std::move(dimList);
    this->valueList = std::move(valueList);
    this->paramNum = -1;
    this->irValue = nullptr;
    this->curTable = nullptr;
    this->returnType = -1;
}

std::string Symbol::getTokenValue() {
    return this->tokenName;
}

std::string Symbol::getName() {
    return this->tokenName;
}

int Symbol::getKind() {
    return this->kind;
}

int Symbol::getSymbolType() {
    return this->type;
}

int Symbol::getParaNum() {
    return this->paramNum;
}

std::vector<int> Symbol::getParaTypes() {
    return this->paraTypes;
}

int Symbol::getReturnType() {
    return this->returnType;
}


void Symbol::setIrValue(IrValue *irValue) {
    this->irValue = irValue;
}

IrValue *Symbol::getIrValue() {
    return this->irValue;
}

int Symbol::calInit() {
    return 114514;
}

int Symbol::calInit(int x) {
    return 1919810;
}

int Symbol::calInit(int x, int y) {
    return 666666;
}

std::vector<int> Symbol::getDimList() {
    return this->dimList;
}

int Symbol::getDim() {
    return this->dim;
}

void Symbol::setDimList(int dim) {
    this->dimList.emplace_back(dim);
}

void Symbol::setDimList(std::vector<int> dimList) {
    for(int i : dimList) {
        this->dimList.emplace_back(i);
    }
}

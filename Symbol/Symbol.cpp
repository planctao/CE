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

    this->kind = VAR_KIND;
}

std::string Symbol::getTokenValue() {
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
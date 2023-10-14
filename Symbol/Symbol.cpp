//
// Created by me on 2023/10/10.
//

#include "Symbol.h"

Symbol::Symbol(int id,int tableId,std::string tokenValue,int type,int kind){
    this->id = id;
    this->tableId = tableId;
    this->tokenName = std::move(tokenValue);
    this->type =
    this->kind = kind;
}

std::string Symbol::getTokenValue() {
    return this->tokenName;
}
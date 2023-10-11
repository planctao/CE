//
// Created by me on 2023/10/10.
//

#include "Symbol.h"

Symbol::Symbol(int id,int tableId,std::string tokenValue,int kind){
    this->id = id;
    this->tableId = tableId;
    this->tokenValue = std::move(tokenValue);
    this->kind = kind;
}
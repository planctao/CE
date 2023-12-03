//
// Created by me on 2023/11/9.
//

#include "SymbolFunc.h"

#include <utility>

SymbolFunc::SymbolFunc(std::string identName, IrValue *irValue, SymbolTable *table) {
    this->tokenName = std::move(identName);
    this->irValue = irValue;
    this->curTable = table;
}

void SymbolFunc::addParamSymbol(Symbol *paramSb) {
    this->paramList.emplace_back(paramSb);
}

//
// Created by me on 2023/11/8.
//

#include "SymbolVar.h"

#include <utility>

SymbolVar::SymbolVar(std::string identName, IrValue *irValue, SymbolTable *curTable) {
    this->tokenName = std::move(identName);
    this->irValue = irValue;
    this->curTable = curTable;
    this->dim = 0;
    this->kind = VAR_KIND;
    this->type = VARIABLE_TYPE;
}

SymbolVar::SymbolVar(std::string identName, IrValue *irValue, SymbolTable *curTable, int dim) {
    this->tokenName = std::move(identName);
    this->irValue = irValue;
    this->curTable = curTable;
    this->dim = 1;
    this->dimList.emplace_back(dim);
    this->kind = VAR_KIND;
    this->type = ARRAY_1_TYPE;
}

SymbolVar::SymbolVar(std::string identName, IrValue *irValue, SymbolTable *curTable, int dim1, int dim2) {
    this->tokenName = std::move(identName);
    this->irValue = irValue;
    this->curTable = curTable;
    this->dim = 2;
    this->dimList.emplace_back(dim1);
    this->dimList.emplace_back(dim2);
    this->kind = VAR_KIND;
    this->type = ARRAY_2_TYPE;
}

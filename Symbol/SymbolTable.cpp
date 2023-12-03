//
// Created by me on 2023/10/10.
//

#include "SymbolTable.h"

#include <utility>
#include <iostream>
SymbolTable::SymbolTable(int id,int parentId){
    this->id = id;
    this->parentId = parentId;
    this->parentTable = nullptr;//无意义
}

SymbolTable::SymbolTable(int id,SymbolTable*parentTable,std::string scopeName) {
    this->id = id;
    this->parentTable = parentTable;
    this->parentId = -2;//无意义
    this->useId = 0;
    this->scopeName = std::move(scopeName);
}

int SymbolTable::getId() {
    return this->id;
}

int SymbolTable::getParentId() {
    return this->parentId;
}

SymbolTable* SymbolTable::getParentTable() {
    return this->parentTable;
}

void SymbolTable::addSymbol(Symbol symbol) {
    std::string name = symbol.getTokenValue();
    this->directory.insert(std::pair<std::string,Symbol>(name,symbol));
}

std::map<std::string,Symbol> SymbolTable::getDirectory(){
    return this->directory;
}

std::map<std::string,Symbol*> SymbolTable::getSymbolList() {
    return this->symbolList;
}

void SymbolTable::addSymbol(Symbol *symbol) {
    std::string name = symbol->getName();
    this->symbolList.insert(std::make_pair(name,symbol));
}

Symbol *SymbolTable::findSymbol(std::string sbName) {
    auto mapIter = this->symbolList.find(sbName);
    if (mapIter!=this->symbolList.end()) {
        return symbolList[sbName];
    }
    if (this->parentTable == nullptr) {
        return nullptr;
    }
    return this->parentTable->findSymbol(sbName);
}

SymbolTable::SymbolTable(SymbolTable *fatherTable) {
    this->parentTable = fatherTable;
    this->id = -2;
    this->parentId = -2;
    this->scopeName = "";
    this->useId = -1;
}

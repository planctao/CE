//
// Created by me on 2023/10/10.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable(int id,int parentId){
    this->id = id;
    this->parentId = parentId;
}

int SymbolTable::getId() {
    return this->id;
}

int SymbolTable::getParentId() {
    return this->parentId;
}

void SymbolTable::addSymbol(Symbol symbol) {
    std::string name = symbol.getTokenValue();
    this->directory.insert(std::pair<std::string,Symbol>(name,symbol));
}

std::map<std::string,Symbol> SymbolTable::getDirectory(){
    return this->directory;
}
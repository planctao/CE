//
// Created by me on 2023/10/10.
//

#include "SymbolTable.h"

SymbolTable::SymbolTable(int id,int parentId){
    this->id = id;
    this->parentId = parentId;
}

void SymbolTable::addSymbol(Symbol symbol) {
    std::string name = symbol.getTokenValue();

}

std::map<std::string,Symbol> SymbolTable::getDirectory(){
    return this->directory;
}
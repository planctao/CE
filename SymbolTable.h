//
// Created by me on 2023/10/10.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H
#include"Symbol.h"
#include<map>
class SymbolTable {
private:
    int id;
    int parentId;
    std::map<std::string,Symbol> directory;
public:
    SymbolTable(int id,int parentId);
    void addSymbol(Symbol symbol);
    std::map<std::string,Symbol> getDirectory();
};


#endif //COMPILER_SYMBOLTABLE_H

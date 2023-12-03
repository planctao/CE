//
// Created by me on 2023/10/10.
//

#ifndef COMPILER_SYMBOLTABLE_H
#define COMPILER_SYMBOLTABLE_H
#include"Symbol.h"
#include<map>
//class Symbol;
class SymbolTable {
private:
    int id;
    int parentId;
    int useId;
    std::string scopeName;
    SymbolTable* parentTable;
    std::map<std::string,Symbol> directory;
    std::map<std::string,Symbol*> symbolList;
public:
    SymbolTable(int id,int parentId);
    SymbolTable(int id,SymbolTable*parentTable,std::string scopeName);
    SymbolTable(SymbolTable* fatherTable);
    SymbolTable() = default;
    void addSymbol(Symbol symbol);
    void addSymbol(Symbol *symbol);
    int getId();
    int getParentId();
    SymbolTable* getParentTable();
    std::map<std::string,Symbol> getDirectory();
    std::map<std::string,Symbol*> getSymbolList();
    Symbol* findSymbol(std::string sbName);
};


#endif //COMPILER_SYMBOLTABLE_H

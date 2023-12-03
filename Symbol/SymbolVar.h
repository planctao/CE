//
// Created by me on 2023/11/8.
//

#ifndef COMPILER_SYMBOLVAR_H
#define COMPILER_SYMBOLVAR_H

#include "Symbol.h"
#include <string>
#include <vector>
class SymbolVar : public Symbol{
public:
    SymbolVar(std::string identName,IrValue* irValue,SymbolTable *curTable);
    SymbolVar(std::string identName,IrValue* irValue,SymbolTable *curTable,int dim);
    SymbolVar(std::string identName,IrValue* irValue,SymbolTable *curTable,int dim1,int dim2);
};


#endif //COMPILER_SYMBOLVAR_H

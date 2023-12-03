//
// Created by me on 2023/11/9.
//

#ifndef COMPILER_SYMBOLFUNC_H
#define COMPILER_SYMBOLFUNC_H

#include <vector>
#include "SymbolVar.h"
#include "Symbol.h"
class SymbolFunc : public Symbol{
protected:
    std::vector<Symbol*> paramList;

public :
    SymbolFunc(std::string identName,IrValue*irValue,SymbolTable*table);
    void addParamSymbol(Symbol* paramSb);
};


#endif //COMPILER_SYMBOLFUNC_H

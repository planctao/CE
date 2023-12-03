//
// Created by me on 2023/11/8.
//

#ifndef COMPILER_SYMBOLCON_H
#define COMPILER_SYMBOLCON_H

#include "Symbol.h"
#include <vector>
class SymbolCon : public Symbol{
protected:
    int init;
    std::vector<int> initArray1;
    std::vector<std::vector<int>> initArray2;

public:
    SymbolCon(std::string identName,int init,IrValue *value,SymbolTable *curTable);
    SymbolCon(std::string identName,std::vector<int> init,IrValue *value,SymbolTable *curTable);
    SymbolCon(std::string identName,std::vector<std::vector<int>> init,IrValue *value,SymbolTable *curTable);
    int calInit() override;
    int calInit(int x) override;
    int calInit(int x,int y) override;
};


#endif //COMPILER_SYMBOLCON_H

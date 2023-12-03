//
// Created by me on 2023/11/1.
//

#ifndef COMPILER_PARAM_H
#define COMPILER_PARAM_H

#include <vector>
#include "../IrValue.h"
#include "../../GrammarTree/Node.h"
#include "../../Symbol/SymbolVar.h"
#include "../../Symbol/Symbol.h"
class Param : public IrValue{
protected:
    int dim;
    int lastDim;
    Node *curNode;
    SymbolTable* curTable;
    int *cnt;
public:
    Param(Node *curNode, SymbolTable *curTable,int *cnt);
    void buildParam();
    Symbol *genParamSymbol();
    int getDim();
    virtual std::string irOutput() override;
};


#endif //COMPILER_PARAM_H

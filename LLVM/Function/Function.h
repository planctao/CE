//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_FUNCTION_H
#define COMPILER_FUNCTION_H

#include <vector>
#include <string>
#include "../IrValue.h"
#include "../BasicBlock/BasicBlock.h"
#include "../../Symbol/SymbolTable.h"
#include "../../GrammarTree/Node.h"
#include "Param.h"

class Function : public IrValue{
protected:
    std::vector<Param*> paramList;
    std::vector<BasicBlock*> basicBlockList;
    SymbolTable *curTable;
    Node *curNode;
    //Module *parentModule;
    std::string returnType;//i32 or void
    int *identCnt;
public:
    Function(std::string tmpName,Node *curNode,SymbolTable *curTable);
    Function(Node*curNode,SymbolTable *curTable);
    void buildFunction();
    void buildFuncParam(Node *tmpNode,SymbolTable* newTable);
    void buildFuncSymbol();
    std::string getReturnType();
    std::vector<Param*> getParamList();
    std::vector<BasicBlock*> getBasicBlock();
    virtual std::string irOutput() override;
};


#endif //COMPILER_FUNCTION_H

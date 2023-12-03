//
// Created by me on 2023/11/9.
//

#ifndef COMPILER_FUNCTIONBUILDER_H
#define COMPILER_FUNCTIONBUILDER_H

#include <vector>
#include <string>
#include "Function.h"
#include "GlobalVar.h"
#include "Param.h"
#include "../../Symbol/SymbolCon.h"
#include "../../Symbol/SymbolVar.h"
class FunctionBuilder {
protected:
    std::vector<Function*> functionList;
    std::vector<GlobalVar*> globalVarList;
    SymbolTable *curTable;
    Node *curNode;
public:
    FunctionBuilder(SymbolTable *curTable,Node *curNode);
    void gen();
    void genGlobalVar(Node *tmpNode);
    void genFunction(Node *tmpNode);
    std::vector<Function*> getFunctionList();
    std::vector<GlobalVar*> getGlobalList();
};


#endif //COMPILER_FUNCTIONBUILDER_H

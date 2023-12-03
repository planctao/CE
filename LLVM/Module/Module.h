//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_MODULE_H
#define COMPILER_MODULE_H

#include <vector>
#include "../IrValue.h"
#include "../Function/Function.h"
#include "../Function/GlobalVar.h"
#include "../../GrammarTree/Node.h"

class Module : public IrValue {
protected:
    std::vector<Function*> functionList;
    std::vector<GlobalVar*> globalVarList;
    Node *curNode;
    SymbolTable *curTable;
public:
    Module(Node *curNode);
    void genCompUnit();
    virtual std::string irOutput() override;
    void fileOutput(std::string ans);
    std::vector<Function*> getFunctionList();
    std::vector<GlobalVar*> getGlobalVarList();
};


#endif //COMPILER_MODULE_H

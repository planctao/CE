//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSFUNCTION_H
#define COMPILER_MIPSFUNCTION_H

#include <vector>
#include <string>
#include "../BasicBlock/MipsBasicBlock.h"
#include "../../LLVM/Function/Function.h"
#include "../MipsSymbolTable.h"
class MipsFunction {
protected:
    Function *irFunction;
    std::vector<MipsBasicBlock*> blockList;
    bool ifMain;
    std::string funcName;
    MipsSymbolTable *mipsSymbolTable;
public:
    explicit MipsFunction(Function *irFunction);
    std::string mipsOutput();
    void genMipsFunc();
};


#endif //COMPILER_MIPSFUNCTION_H

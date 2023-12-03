//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSBLOCKBUILDER_H
#define COMPILER_MIPSBLOCKBUILDER_H

#include "../../LLVM/BasicBlock/BasicBlock.h"
#include "../../LLVM/Function/Function.h"
#include "MipsBasicBlock.h"
class MipsModule;
class MipsBlockBuilder {
protected:
    BasicBlock* irBasicBlock;
    Function* irFunction;
    MipsSymbolTable *mipsSymbolTable;
public:
    explicit MipsBlockBuilder(Function *irFunction,BasicBlock*irBasicBlock,MipsSymbolTable *mipsSymbolTable1);
    MipsBasicBlock *genBasicBlock();
    std::string makeBlockName();
};


#endif //COMPILER_MIPSBLOCKBUILDER_H

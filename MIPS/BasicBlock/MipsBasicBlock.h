//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSBASICBLOCK_H
#define COMPILER_MIPSBASICBLOCK_H
#include <vector>
#include "../Instructioin/MipsInstruction.h"
#include "../MipsSymbolTable.h"
#include "../../LLVM/BasicBlock/BasicBlock.h"
class MipsBasicBlock {
protected:
    std::vector<MipsInstruction*> instructionList;
    MipsSymbolTable *mipsSymbolTable;
    BasicBlock *irBasicBlock;
    std::string blockName;
public:
    void addMipsInstr(MipsInstruction* tmpMipsInstr);
    void addMipsInstr(std::vector<MipsInstruction*> tmpMipsInstrList);
    MipsBasicBlock(std::string blockName,BasicBlock *irTmpBlock,MipsSymbolTable* mipsSymbolTable1);
    std::string mipsOutput() ;
};


#endif //COMPILER_MIPSBASICBLOCK_H

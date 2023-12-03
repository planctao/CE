//
// Created by me on 2023/11/14.
//

#include "MipsBasicBlock.h"

#include <utility>
#include "../Instructioin/MipsInstructionBuilder.h"
void MipsBasicBlock::addMipsInstr(MipsInstruction *tmpMipsInstr) {
    this->instructionList.emplace_back(tmpMipsInstr);
}

void MipsBasicBlock::addMipsInstr(std::vector<MipsInstruction *> tmpMipsInstrList) {
    this->instructionList.insert(this->instructionList.end(),tmpMipsInstrList.begin(),tmpMipsInstrList.end());
}

MipsBasicBlock::MipsBasicBlock(std::string blockName,BasicBlock * irTmpBlock,MipsSymbolTable *mipsSymbolTable1) {
    this->mipsSymbolTable = mipsSymbolTable1;
    this->irBasicBlock = irTmpBlock;
    this->blockName = std::move(blockName);
}

std::string MipsBasicBlock::mipsOutput() {
    std::string ans;
    ans += this->blockName + ":\n";
    for (auto instr:this->instructionList) {
        ans += instr->mipsOutput() + "\n";
    }
    return ans;
}



//
// Created by me on 2023/10/31.
//

#include "BasicBlock.h"

#include <utility>
BasicBlock::BasicBlock(std::string valueName,Node *curNode) : IrValue(std::move(valueName)){
    this->curNode = curNode;
}

BasicBlock::BasicBlock(std::string valueName) : IrValue(std::move(valueName)) {
    this->curNode = nullptr;
}

void BasicBlock::addInstruction(Instruction *tmpInstruction) {
    this->instructionList.emplace_back(tmpInstruction);
}

void BasicBlock::addInstruction(std::vector<Instruction *> tmpInstructions) {
    this->instructionList.insert(this->instructionList.end(),tmpInstructions.begin(),tmpInstructions.end());
}

void BasicBlock::buildBasicBlock() {

}

std::string BasicBlock::irOutput() {
    std::string ans;
    std::string tmpName = this->name;
    tmpName.erase(0,1);//删掉开头的‘%’
    ans += tmpName + ":\n";
    for (auto tmpInstr : this->instructionList) {
        ans += tmpInstr->irOutput() + "\n";
    }
    return ans;
}

std::vector<Instruction *> BasicBlock::getInstrList() {
    return this->instructionList;
}



//
// Created by me on 2023/11/14.
//

#include "MipsBlockBuilder.h"
#include "../../LLVM/Instruction/Instruction.h"

#include "../../LLVM/Instruction/Call.h"
#include "../Module/MipsModule.h"

#include "../Instructioin/MipsInstruction.h"
#include "../Instructioin/MipsInstructionBuilder.h"
#include "../Instructioin/La.h"
#include "../Instructioin/Li.h"
#include "../Instructioin/Syscall.h"
#include "../Function/Asciiz.h"
MipsBlockBuilder::MipsBlockBuilder(Function* irFunction,BasicBlock *irBasicBlock,MipsSymbolTable *mipsSymbolTable1) {
    this->irBasicBlock = irBasicBlock;
    this->mipsSymbolTable = mipsSymbolTable1;
    this->irFunction = irFunction;
}

MipsBasicBlock *MipsBlockBuilder::genBasicBlock() {
    std::vector<Instruction*> instrList = this->irBasicBlock->getInstrList();
    std::string blockName = makeBlockName();
    MipsBasicBlock *mipsBasicBlock = new MipsBasicBlock(blockName,irBasicBlock,this->mipsSymbolTable);
    for(int i = 0;i < instrList.size();i++) {
        Instruction *tmpInstr = instrList.at(i);
        if (tmpInstr->isCallPutCh()) {
            std::string asciiStr;
            asciiStr += "\"";
            while(i < instrList.size() && instrList.at(i)->isCallPutCh()) {
                Call *tmpCall = dynamic_cast<Call*>(instrList.at(i));
                if (tmpCall->getPutName() == "10") {
                    asciiStr += "\\n";
                }
                else {
                    asciiStr += (char)(std::stoi(tmpCall->getPutName()));
                }
                i++;
            }
            int index = MipsModule::getAsciizSize();
            asciiStr += "\"";
            Asciiz *asciiz = new Asciiz("str" + std::to_string(index),asciiStr);
            MipsModule::addAsciiz(asciiz);
            La *la = new La(strReg2intReg("$a0"),"str" + std::to_string(index));
            Li* li = new Li(strReg2intReg("$v0"),4);
            Syscall *syscall = new Syscall();
            mipsBasicBlock->addMipsInstr(la);
            mipsBasicBlock->addMipsInstr(li);
            mipsBasicBlock->addMipsInstr(syscall);
            i--;
        }
        else {
             MipsInstructionBuilder *mipsInstrBuilder = new MipsInstructionBuilder(tmpInstr,this->mipsSymbolTable);
             mipsInstrBuilder->genMipsInstr();
             mipsBasicBlock->addMipsInstr(mipsInstrBuilder->getMipsInstrList());
        }
    }
    return mipsBasicBlock;
}

std::string MipsBlockBuilder::makeBlockName() {
    std::string ans;
    std::string funcName = this->irFunction->getName();
    funcName.erase(0,1);//去除@
    std::string blockName = this->irBasicBlock->getName();
    blockName.erase(0,1);//去除%
    ans = funcName + "_" + blockName;
    return ans;
}

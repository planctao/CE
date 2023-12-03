//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_BASICBLOCK_H
#define COMPILER_BASICBLOCK_H

#include <vector>
#include <string>
#include "../IrValue.h"
#include "../Instruction/Instruction.h"
#include "../../GrammarTree/Node.h"
class BasicBlock : public IrValue{
protected:
    std::vector<Instruction*> instructionList;
    Node *curNode;
public:
    BasicBlock(std::string valueName, Node*curNode);
    BasicBlock(std::string valueName);
    void buildBasicBlock();
    void addInstruction(Instruction *tmpInstruction);
    void addInstruction(std::vector<Instruction*> tmpInstructions);
    std::vector<Instruction*> getInstrList();
    virtual std::string irOutput() override;
};


#endif //COMPILER_BASICBLOCK_H

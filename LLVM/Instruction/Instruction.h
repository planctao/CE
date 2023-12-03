//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_INSTRUCTION_H
#define COMPILER_INSTRUCTION_H

#include "../IrValue.h"
#include "../IrUser.h"
#include "../../Util/InstructionType.h"

class Instruction : public IrUser{
protected:
    InstructionType type;

public:
    Instruction();
    InstructionType getInstrType();
    bool isCallPutCh();
    std::string irOutput() override;
};


#endif //COMPILER_INSTRUCTION_H

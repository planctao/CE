//
// Created by me on 2023/10/31.
//

#include "Instruction.h"
#include "Call.h"
Instruction::Instruction() :IrUser(0){
    this->type = InstructionType :: NULLTYPE;
}

std::string Instruction::irOutput() {
    return "Instruction";
}

InstructionType Instruction::getInstrType() {
    return this->type;
}

bool Instruction::isCallPutCh() {
    if (this->type == InstructionType::CALL) {
        Call* call = dynamic_cast<Call*>(this);
        if (call->getFuncType() == CALL_PUTCH){
            return true;
        }
    }
    return false;
}


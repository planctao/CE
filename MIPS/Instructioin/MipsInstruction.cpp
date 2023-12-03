//
// Created by me on 2023/11/14.
//

#include "MipsInstruction.h"

MipsInstruction::MipsInstruction(MipsInstrType mipsInstrType) {
    this->mipsInstrType = mipsInstrType;
}

std::string MipsInstruction::mipsOutput() {
    std::cout << "it is mipsInstruction\n";
    return "it is mipsInstruction\n";
}

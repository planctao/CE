//
// Created by me on 2023/11/17.
//

#include "Addi.h"

Addi::Addi(int dstReg, int srcReg, int immediate) : MipsInstruction(MipsInstrType::ADDI){
    this->dstReg = dstReg;
    this->srcReg = srcReg;
    this->immediate = immediate;
}

std::string Addi::mipsOutput() {
    std::string ans;
    ans += "addi " + intReg2strReg(dstReg) + " "+ intReg2strReg(srcReg) + " " + std::to_string(immediate);
    return ans;
}

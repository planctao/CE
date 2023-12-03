//
// Created by me on 2023/11/17.
//

#include "Mfhi.h"

Mfhi::Mfhi(int tarReg) : MipsInstruction(MipsInstrType::MFHI){
    this->tarReg = tarReg;
}

std::string Mfhi::mipsOutput() {
    std::string ans;
    ans += "mfhi " + intReg2strReg(tarReg);
    return ans;
}

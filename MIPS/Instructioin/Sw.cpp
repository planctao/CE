//
// Created by me on 2023/11/16.
//

#include "Sw.h"
#include "../../Util/utils.h"

Sw::Sw(int tarReg, int offset, int base) : MipsInstruction(MipsInstrType::SW){
    this->tarReg = tarReg;
    this->offset = offset;
    this->base = base;
}

Sw::Sw(std::string tarReg, int offset, std::string baseReg) : MipsInstruction(MipsInstrType::SW) {
    this->tarReg = strReg2intReg(std::move(tarReg));
    this->offset = offset;
    this->base = strReg2intReg(std::move(baseReg));
}

std::string Sw::mipsOutput() {
    std::string ans;
    ans += "sw " + intReg2strReg(tarReg) + " " + std::to_string(offset) + "(" + intReg2strReg(base) + ")";
    return ans;
}

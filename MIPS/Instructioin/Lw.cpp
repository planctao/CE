//
// Created by me on 2023/11/16.
//

#include "Lw.h"

#include <utility>
#include "../../Util/utils.h"
Lw::Lw(int tarReg, int offset, int base) : MipsInstruction(MipsInstrType::LW){
    this->tarReg = tarReg;
    this->offset = offset;
    this->base = base;
}

Lw::Lw(std::string tarReg, int offset, std::string baseReg) : MipsInstruction(MipsInstrType::LW) {
    this->tarReg = strReg2intReg(std::move(tarReg));
    this->offset = offset;
    this->base = strReg2intReg(std::move(baseReg));
}

std::string Lw::mipsOutput() {
    std::string ans;
    ans += "lw " + intReg2strReg(tarReg) + " " + std::to_string(offset) + "(" + intReg2strReg(base) + ")";
    return ans;
}


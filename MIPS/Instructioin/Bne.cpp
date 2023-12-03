//
// Created by me on 2023/11/29.
//

#include "Bne.h"
Bne::Bne(int reg1, int reg2, std::string label) : MipsInstruction(MipsInstrType::BNE){
    this->reg1 = reg1;
    this->reg2 = reg2;
    this->label = std::move(label);
}

std::string Bne::mipsOutput() {
    std::string ans;
    ans += "bne ";
    ans += intReg2strReg(reg1) + ", ";
    ans += intReg2strReg(reg2) + ", ";
    ans += this->label;
    return ans;
}
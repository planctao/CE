//
// Created by me on 2023/11/29.
//

#include "Beq.h"

Beq::Beq(int reg1, int reg2, std::string label) : MipsInstruction(MipsInstrType::BEQ){
    this->reg1 = reg1;
    this->reg2 = reg2;
    this->label = std::move(label);
}

std::string Beq::mipsOutput() {
    std::string ans;
    ans += "beq ";
    ans += intReg2strReg(reg1) + ", ";
    ans += intReg2strReg(reg2) + ", ";
    ans += this->label;
    return ans;
}

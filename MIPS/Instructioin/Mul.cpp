//
// Created by me on 2023/11/17.
//

#include "Mul.h"

Mul::Mul(int dstReg, int src1Reg, int src2Reg) : MipsInstruction(MipsInstrType::MUL){
    this->dstReg = dstReg;
    this->src1Reg = src1Reg;
    this->src2Reg = src2Reg;
}

std::string Mul::mipsOutput() {
    std::string ans;
    ans += "mul " + intReg2strReg(dstReg) + " " + intReg2strReg(src1Reg) + " " + intReg2strReg(src2Reg);
    return ans;
}
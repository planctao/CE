//
// Created by me on 2023/11/17.
//

#include "Sub.h"

Sub::Sub(int dstReg, int src1Reg, int src2Reg) : MipsInstruction(MipsInstrType::SUB){
    this->dstReg = dstReg;
    this->src1Reg = src1Reg;
    this->src2Reg = src2Reg;
}

std::string Sub::mipsOutput() {
    std::string ans;
    ans += "sub " + intReg2strReg(dstReg) + " " + intReg2strReg(src1Reg) + " " + intReg2strReg(src2Reg);
    return ans;
}
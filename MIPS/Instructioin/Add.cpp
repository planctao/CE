//
// Created by me on 2023/11/17.
//

#include "Add.h"

Add::Add(int dstReg, int src1Reg, int src2Reg) : MipsInstruction(MipsInstrType::ADD){
    this->dstReg = dstReg;
    this->src1Reg = src1Reg;
    this->src2Reg = src2Reg;
}

std::string Add::mipsOutput() {
    std::string ans;
    ans += "add " + intReg2strReg(dstReg) + " " + intReg2strReg(src1Reg) + " " + intReg2strReg(src2Reg);
    return ans;
}


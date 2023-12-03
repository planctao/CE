//
// Created by me on 2023/11/17.
//

#include "Div.h"
Div::Div(int dstReg, int src1Reg, int src2Reg) : MipsInstruction(MipsInstrType::DIV){
    this->dstReg = dstReg;
    this->src1Reg = src1Reg;
    this->src2Reg = src2Reg;
}

std::string Div::mipsOutput() {
    std::string ans;
    if (dstReg == -1) {
        ans = "div " + intReg2strReg(src1Reg) + " " + intReg2strReg(src2Reg);
    }
    else {
        ans = "div " + intReg2strReg(dstReg) + " " + intReg2strReg(src1Reg) + " " + intReg2strReg(src2Reg);
    }
    return ans;
}

//
// Created by me on 2023/11/29.
//

#include "Cmp.h"

Cmp::Cmp(int cmpType, int dstReg, int leftReg, int rightReg)
        : MipsInstruction(MipsInstrType::CMP) {
    this->cmpType = cmpType;
    this->dstReg = dstReg;
    this->src1Reg = leftReg;
    this->src2Reg = rightReg;
}

std::string Cmp::mipsOutput() {
    std::string ans;
    ans += cmpType2Str() + " ";
    ans += intReg2strReg(dstReg) + ", ";
    ans += intReg2strReg(src1Reg) + ", ";
    ans += intReg2strReg(src2Reg);
    return ans;
}

std::string Cmp::cmpType2Str() {
    if (this->cmpType == CMP_EQ) return "seq";
    if (this->cmpType == CMP_NE) return "sne";
    if (this->cmpType == CMP_SGT) return "sgt";
    if (this->cmpType == CMP_SGE) return "sge";
    if (this->cmpType == CMP_SLT) return "slt";
    if (this->cmpType == CMP_SLE) return "sle";
    std::cout << "cmpType2Str error\n";
    return "";
}

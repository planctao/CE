//
// Created by me on 2023/11/17.
//

#include "Move.h"

Move::Move(int dstReg, int srcReg) : MipsInstruction(MipsInstrType::MOVE){
    this->dstReg = dstReg;
    this->srcReg = srcReg;
}

std::string Move::mipsOutput() {
    std::string ans;
    ans += "move " + intReg2strReg(dstReg) + " "+ intReg2strReg(srcReg);
    return ans;
}

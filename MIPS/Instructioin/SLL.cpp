//
// Created by me on 2023/11/28.
//

#include "SLL.h"
#include "../../Util/utils.h"

SLL::SLL(int tarReg, int srcReg, int offset) : MipsInstruction(MipsInstrType::SLL){
    this->tarReg = tarReg;
    this->srcReg = srcReg;
    this->offset = offset;
}

std::string SLL::mipsOutput() {
    std::string ans;
    ans = "sll " + intReg2strReg(tarReg) + ", " + intReg2strReg(srcReg) + ", " + std::to_string(offset);
    return ans;
}



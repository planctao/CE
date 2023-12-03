//
// Created by me on 2023/11/14.
//

#include "La.h"

#include <utility>


La::La(char regType, int regNum, std::string label) : MipsInstruction(MipsInstrType::LA){
    this->regType = regType;
    this->regIndex = regNum;
    this->label = std::move(label);
}

La::La(int regNum, std::string label) : MipsInstruction(MipsInstrType::LA) {
    this->regNum = regNum;
    this->label = std::move(label);
}

std::string La::mipsOutput() {
    std::string ans;
    ans += "la " + intReg2strReg(regNum) + " " + label;
    return ans;
}

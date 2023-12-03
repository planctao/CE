//
// Created by me on 2023/11/14.
//

#include "Li.h"
#include <regex>
Li::Li(char get, int regIndex, int immediateNum) : MipsInstruction(MipsInstrType::LI){
    this->regType = get;
    this->regIndex = regIndex;
    this->immediateNum = immediateNum;
    this->regNum = -1;
}

Li::Li(std::string regName, int immediate) : MipsInstruction(MipsInstrType::LI){
    if (regName[0] == '$') {
        this->regType = regName[1];
        this->regIndex = regName[2] - '0';
    }
    else {
        this->regType = regName[0];
        this->regIndex = regName[1] - '0';
    }
    this->immediateNum = immediate;
    this->regNum = -1;
}

Li::Li(int regNum, int immediateNum) : MipsInstruction(MipsInstrType::LI) {
    this->regNum = regNum;
    this->immediateNum = immediateNum;
    /*no mean*/
    this->regType = '0';
    this->regIndex = -1;
}

std::string Li::mipsOutput() {
    std::string ans;
    ans += "li " + intReg2strReg(regNum) + " " + std::to_string(immediateNum);
    return ans;
}



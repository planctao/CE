//
// Created by me on 2023/11/17.
//

#ifndef COMPILER_DIV_H
#define COMPILER_DIV_H

#include "MipsInstruction.h"
class Div : public MipsInstruction{
protected:
    int dstReg;
    int src1Reg;
    int src2Reg;
public:
    Div(int dstReg,int src1Reg,int src2Reg);
    std::string mipsOutput() override;
};


#endif //COMPILER_DIV_H

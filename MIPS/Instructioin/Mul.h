//
// Created by me on 2023/11/17.
//

#ifndef COMPILER_MUL_H
#define COMPILER_MUL_H

#include "MipsInstruction.h"
class Mul : public MipsInstruction{
protected:
    int dstReg;
    int src1Reg;
    int src2Reg;
public:
    Mul(int dstReg,int src1Reg,int src2Reg);
    std::string mipsOutput() override;
};


#endif //COMPILER_MUL_H

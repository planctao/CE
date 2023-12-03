//
// Created by me on 2023/11/17.
//

#ifndef COMPILER_SUB_H
#define COMPILER_SUB_H
#include "MipsInstruction.h"


class Sub : public MipsInstruction{
protected:
    int dstReg;
    int src1Reg;
    int src2Reg;
public:
    Sub(int dstReg,int src1Reg,int src2Reg);
    std::string mipsOutput() override;
};


#endif //COMPILER_SUB_H

//
// Created by me on 2023/11/17.
//

#ifndef COMPILER_ADD_H
#define COMPILER_ADD_H

#include "MipsInstruction.h"
class Add : public MipsInstruction{
protected:
    int dstReg;
    int src1Reg;
    int src2Reg;
public:
    Add(int dstReg,int src1Reg,int src2Reg);
    std::string mipsOutput() override;
};


#endif //COMPILER_ADD_H

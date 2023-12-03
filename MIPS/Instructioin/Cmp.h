//
// Created by me on 2023/11/29.
//

#ifndef COMPILER_CMP_H
#define COMPILER_CMP_H
#include "MipsInstruction.h"
#define CMP_EQ 0
#define CMP_NE 1
#define CMP_SGT 2
#define CMP_SGE 3
#define CMP_SLT 4
#define CMP_SLE 5

class Cmp : public MipsInstruction{
protected:
    int cmpType;
    int src1Reg;
    int src2Reg;
    int dstReg;
public:
    Cmp(int cmpType, int dstReg, int leftReg, int rightReg);
    std::string mipsOutput() override;
    std::string cmpType2Str();
};


#endif //COMPILER_CMP_H

//
// Created by me on 2023/11/17.
//

#ifndef COMPILER_ADDI_H
#define COMPILER_ADDI_H

#include "MipsInstruction.h"

class Addi : public MipsInstruction{
protected:

    int dstReg;
    int srcReg;
    int immediate;
public:
    Addi(int dstReg,int srcReg,int immediate);
    std::string mipsOutput() override;
};


#endif //COMPILER_ADDI_H

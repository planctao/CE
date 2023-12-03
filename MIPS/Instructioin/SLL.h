//
// Created by me on 2023/11/28.
//

#ifndef COMPILER_SLL_H
#define COMPILER_SLL_H

#include "MipsInstruction.h"

class SLL : public MipsInstruction{
protected:
    int tarReg;
    int srcReg;
    int offset;
public:
    SLL(int tarReg,int srcReg,int offset);
    std::string mipsOutput() override;
};


#endif //COMPILER_SLL_H

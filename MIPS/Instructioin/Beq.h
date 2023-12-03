//
// Created by me on 2023/11/29.
//

#ifndef COMPILER_BEQ_H
#define COMPILER_BEQ_H

#include "MipsInstruction.h"

class Beq : public MipsInstruction{
protected:
    int reg1;
    int reg2;
    std::string label;
public:
    Beq(int reg1,int reg2,std::string label);
    std::string mipsOutput() override;
};


#endif //COMPILER_BEQ_H

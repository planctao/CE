//
// Created by me on 2023/11/29.
//

#ifndef COMPILER_BNE_H
#define COMPILER_BNE_H

#include "MipsInstruction.h"
#include <string>
class Bne : public MipsInstruction{
protected:
    int reg1;
    int reg2;
    std::string label;
public:
    Bne(int reg1,int reg2,std::string label);
    std::string mipsOutput() override;
};


#endif //COMPILER_BNE_H

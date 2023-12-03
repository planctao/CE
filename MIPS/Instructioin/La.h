//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_LA_H
#define COMPILER_LA_H

#include "MipsInstruction.h"
class La : public MipsInstruction{
protected:
    char regType;//v,a,t,s
    int regIndex;
    std::string label;
    int regNum;
public:
    La(char regType,int regNum,std::string label);
    La(int regNum,std::string label);
    std::string mipsOutput() override;
};


#endif //COMPILER_LA_H

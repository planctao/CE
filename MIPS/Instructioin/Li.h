//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_LI_H
#define COMPILER_LI_H

#include "MipsInstruction.h"
#define LI_PRINT_INT 1
#define LI_PRINT_STR 4
#define LI_READ_INT 5
#define LI_EXIT 10
class Li : public MipsInstruction{
protected:
    char regType;
    int regIndex;
    int regNum;
    int immediateNum;
public:
    Li(char get,int regIndex,int immediateNum);
    Li(std::string regName,int immediate);
    Li (int regNum,int immediateNum);
    std::string mipsOutput() override;
};


#endif //COMPILER_LI_H

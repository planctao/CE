//
// Created by me on 2023/11/16.
//

#ifndef COMPILER_LW_H
#define COMPILER_LW_H

#include <string>
#include "MipsInstruction.h"

class Lw : public MipsInstruction{
protected:
    int tarReg;//存的内容
    int offset;//偏移量
    int base;//基址寄存器
public:
    Lw(int tarReg,int offset,int base);
    Lw(std::string tarReg,int offset,std::string baseReg);
    std::string mipsOutput() override;
};


#endif //COMPILER_LW_H

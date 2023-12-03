//
// Created by me on 2023/11/16.
//

#ifndef COMPILER_SW_H
#define COMPILER_SW_H

#include "MipsInstruction.h"

class Sw : public MipsInstruction{
protected:
    int tarReg;//存的内容
    int offset;//偏移量
    int base;//基址寄存器
public:
    Sw(int tarReg,int offset,int base);
    Sw(std::string tarReg,int offset,std::string baseReg);
    std::string mipsOutput() override;
};


#endif //COMPILER_SW_H

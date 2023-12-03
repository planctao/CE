//
// Created by me on 2023/11/4.
//

#ifndef COMPILER_ALLOCINSTRUCTION_H
#define COMPILER_ALLOCINSTRUCTION_H

#include "Instruction.h"
#include "../IrValue.h"
#include "../Use.h"
#include <string>
class AllocInstruction : public Instruction{
protected:
    bool ifInit;
    IrValue* irValue;
    int dim1;
    int dim2;
public:
    AllocInstruction(bool ifInit,IrValue *irValue,int dim1,int dim2);
    AllocInstruction(bool ifInit,std::string irName);
    virtual std::string irOutput();
};


#endif //COMPILER_ALLOCINSTRUCTION_H

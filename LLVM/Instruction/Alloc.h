//
// Created by me on 2023/11/7.
//

#ifndef COMPILER_ALLOC_H
#define COMPILER_ALLOC_H

#include <string>
#include <vector>
#include "Instruction.h"
#include "../ValueType/PointerType.h"
#include "../../Util/InstructionType.h"
class Alloc : public Instruction{
protected:
    PointerType *ptValueType; //指被分配的那个对象
public:
    Alloc(PointerType *valueType);
    Alloc(int arraySize);//是array的size
    Alloc();
    std::string irOutput() override;
    PointerType *getPtValueType();
    ValueType *getPtContainedType();
    int getMemorySize();
};


#endif //COMPILER_ALLOC_H

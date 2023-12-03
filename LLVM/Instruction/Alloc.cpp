//
// Created by me on 2023/11/7.
//

#include "Alloc.h"
#include <iostream>
#include "../ValueType/IntegerType.h"
#include "../ValueType/ArrayType.h"

Alloc::Alloc(PointerType *valueType) {
    this->type = InstructionType::ALLOCA;
    this->ptValueType = valueType;
    this->valueType = valueType;
}

std::string Alloc::irOutput() {
    std::string ans;
    ans += this->name + " = ";
    ans += "alloca ";
    ans += this->ptValueType->containedTypeOutput() + " ";
    return ans;
}

PointerType *Alloc::getPtValueType() {
    return this->ptValueType;
}

ValueType *Alloc::getPtContainedType() {
    return this->ptValueType->getContained();
}

Alloc::Alloc(int arraySize) {
    this->type = InstructionType::ALLOCA;
    IntegerType *integerType = new IntegerType(32);
    ArrayType* arrayType = new ArrayType(arraySize,integerType);
    PointerType* pt = new PointerType(arrayType);
    this->ptValueType = pt;
    this->valueType = pt;
}

Alloc::Alloc() {
    this->type = InstructionType::ALLOCA;
    IntegerType *integer = new IntegerType(32);
    PointerType* pt = new PointerType(integer);
    this->ptValueType = pt;
    this->valueType = pt;
}

int Alloc::getMemorySize() {
    if (this->ptValueType->getContainedIrValueType() == IrValueType::INTEGER) {
        return 4;
    }
    else if (this->ptValueType->getContainedIrValueType() == IrValueType::ARRAY){
       ArrayType *arrayType = dynamic_cast<ArrayType*>(this->ptValueType->getContained());
       return 4 * arrayType->getElementNum();
    }
    else {
        std::cout << "Alloc::getMemorySize error\n";
        return -1;
    }
}



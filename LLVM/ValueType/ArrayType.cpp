//
// Created by me on 2023/11/6.
//

#include "ArrayType.h"

ArrayType::ArrayType(int elementNum, ValueType *valueType) : ValueType(IrValueType::ARRAY){
    this-> elementNum = elementNum;
    this->elementType = valueType;
}

std::string ArrayType::typeOutput() {
    return "[ " + std::to_string(this->elementNum) + " x " + this->elementType->typeOutput() + "]";
}

int ArrayType::getElementNum() {
    return this->elementNum;
}



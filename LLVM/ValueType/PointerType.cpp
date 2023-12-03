//
// Created by me on 2023/11/7.
//

#include "PointerType.h"

PointerType::PointerType(ValueType* contained) :  ValueType(IrValueType::POINTER) {
    this->contained = contained;
}

std::string PointerType::typeOutput() {
    return this->contained->typeOutput() + "*";
}

IrValueType PointerType::getContainedIrValueType() {
    return this->contained->getIrValueType();
}

std::string PointerType::containedTypeOutput() {
    return this->contained->typeOutput();
}

ValueType *PointerType::getContained() {
    return this->contained;
}

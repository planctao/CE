//
// Created by me on 2023/10/31.
//

#include "IrValue.h"

#include <utility>
#include "../LLVM/ValueType/IntegerType.h"
#include "../LLVM/ValueType/NumberType.h"

IrValue::IrValue(std::string name){
    this->name = std::move(name);
    this->valueType = nullptr;
    this->ifTrue = true;
}

void IrValue::addUse(Use *tmpUse) {
    this->useList.emplace_back(tmpUse);
}

void IrValue::setName(std::string name) {
    this->name = std::move(name);
}

IrValue::IrValue(std::string name, ValueType *valueType) {
    this->name = std::move(name);
    this->valueType = valueType;
    this->ifTrue = true;

}

void IrValue::setValueType(ValueType* valueType1) {
    this->valueType = valueType1;
}

std::string IrValue::irOutput() {
    return this->name;
}

std::string IrValue::getName() {
    return this->name;
}

ValueType *IrValue::getValueType() {
    return this->valueType;
}

int IrValue::getIntegerTypeValue() {
    IntegerType *integer = dynamic_cast<IntegerType*>(this->valueType);
    if (integer == nullptr) {
        NumberType *number = dynamic_cast<NumberType*>(this->valueType);
        if (number != nullptr) {
            return 32;
        }
    }
    return integer->getIntegerType();
}

bool IrValue::getIfTrue() {
    return this->ifTrue;
}

void IrValue::setIfTrue() {
    this->ifTrue = !this->ifTrue;
}
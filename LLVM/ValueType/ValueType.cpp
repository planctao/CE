//
// Created by me on 2023/11/6.
//

#include "ValueType.h"

ValueType::ValueType(IrValueType type) {
    this->valueType = type;
}

std::string ValueType::typeOutput() {
    return "valueType";
}

IrValueType ValueType::getIrValueType() {
    return this->valueType;
}

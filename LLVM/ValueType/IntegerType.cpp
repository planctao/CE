//
// Created by me on 2023/11/6.
//

#include "IntegerType.h"

IntegerType::IntegerType(int type) : ValueType(IrValueType::INTEGER){
    this->integerType = type;
}

std::string IntegerType::typeOutput() {
    return " i32";
}

int IntegerType::getIntegerType() {
    return this->integerType;
}

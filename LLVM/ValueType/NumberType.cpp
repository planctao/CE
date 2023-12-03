//
// Created by me on 2023/11/7.
//

#include "NumberType.h"

NumberType::NumberType() : ValueType(IrValueType::NUMBER){

}

std::string NumberType::typeOutput() {
    return "i32";
}

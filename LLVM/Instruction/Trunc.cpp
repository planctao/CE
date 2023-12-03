//
// Created by me on 2023/11/23.
//

#include "Trunc.h"
#include "../ValueType/IntegerType.h"

Trunc::Trunc(IrValue *tar) {
    this->type = InstructionType::TRUNC;
    Use *use = new Use(tar,(IrUser*)this,0);
    this->UserUseList.emplace_back(use);
    this->valueType = new IntegerType(1);
}

std::string Trunc::irOutput() {
    std::string ans;
    ans += this->name + " = trunc i32 " + this->UserUseList.at(0)->getValueName() + " to i1";
    return ans;
}

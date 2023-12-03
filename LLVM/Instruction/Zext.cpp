//
// Created by me on 2023/11/23.
//

#include "Zext.h"
#include "../ValueType/IntegerType.h"
Zext::Zext(IrValue *tar) {
    this->type = InstructionType::ZEXT;
    Use *use = new Use(tar,(IrUser*)this,0);
    this->UserUseList.emplace_back(use);
    this->valueType = new IntegerType(32);
}

std::string Zext::irOutput() {
    std::string ans;
    ans += this->name + " =  zext i1 " + this->UserUseList.at(0)->getValueName() + " to i32";
    return ans;
}

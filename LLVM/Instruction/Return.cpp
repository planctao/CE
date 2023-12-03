//
// Created by me on 2023/11/11.
//

#include "Return.h"
#include <iostream>
#include <utility>

Return::Return(IrValue *ret) {
    this->type = InstructionType::RET;
    Use *use = new Use(ret,(IrUser*)this,0);
    this->addUserUseList(use);
}

Return::Return(std::string irName) {
    this->type = InstructionType::RET;
    IrValue* ret = new IrValue(std::move(irName));
    Use *use = new Use(ret,(IrUser*)this,0);
    this->addUserUseList(use);
}

std::string Return::irOutput() {
    std::string ans;
    if (this->UserUseList[0]->getValueName() == "void") {
        ans = "ret void";
        return ans;
    }
    ans += "ret i32 " +  this->UserUseList[0]->getValueName()  + "\n";
    return ans;
}

std::string Return::getRet() {
    return this->UserUseList[0]->getValueName();
}

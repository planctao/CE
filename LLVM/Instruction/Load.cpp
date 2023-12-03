//
// Created by me on 2023/11/7.
//

#include "Load.h"
#include "../ValueType/PointerType.h"
Load::Load(IrValue *tar) {
    this->type = InstructionType::LOAD;
    Use* use = new Use(tar,(IrUser*)(this),0);
    this->opNum = 1;
    //tar->addUse(use);
    this->addUserUseList(use);
    PointerType *pt = dynamic_cast<PointerType*>(tar->getValueType());
    this->valueType = pt->getContained();
}

std::string Load::irOutput() {
    std::string ans;
    PointerType *pt = dynamic_cast<PointerType*>(this->UserUseList[0]->getIrValue()->getValueType());
    ans += this->name + " = " + "load" + " " + pt->containedTypeOutput()
            + ", " + pt->typeOutput()  + " " +
            this->UserUseList[0]->getValueName();
    return ans;
}

std::string Load::getSrc() {
    return this->UserUseList[0]->getValueName();
}

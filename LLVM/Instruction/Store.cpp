//
// Created by me on 2023/11/4.
//

#include "Store.h"

#include <utility>
#include <iostream>
Store::Store(std::string tarValue, std::string dest) : Instruction(){
    this->type = InstructionType::STORE;
    IrValue *tar = new IrValue(std::move(tarValue));
    IrValue *dst = new IrValue(std::move(dest));
    this->opNum = 2;
    IrUser *tmpUser = this;//创建IrUser对象用来个Use类初始化
    Use* useTar = new Use(tar,tmpUser,0);//第一个Use对象用来存放目标数值
    //tar->addUse(useTar);//将IrValue里面放入Use对象
    this->addUserUseList(useTar);
    Use* useDst = new Use(dst,tmpUser,1);
    //dst->addUse(useDst);
    this->addUserUseList(useDst);
}

Store::Store(IrValue *val, IrValue *dst) {
    this->type = InstructionType::STORE;
    this->opNum = 2;
    IrUser *tmpUser = this;
    Use *useVal = new Use(val,tmpUser,0);
    //val->addUse(useVal);
    this->addUserUseList(useVal);
    Use*useDst = new Use(dst,tmpUser,1);
    //dst->addUse(useDst);
    this->addUserUseList(useDst);
}

std::string Store::irOutput() {
    std::string ans;
    IrValue* srcValue = this->UserUseList.at(0)->getIrValue();
    IrValue* dstValue = this->UserUseList.at(1)->getIrValue();
    ans += "store " + srcValue->getValueType()->typeOutput() + " " + this->UserUseList[0]->getValueName() + ", ";
    ans += dstValue->getValueType()->typeOutput()  + " " + this->UserUseList[1]->getValueName();
    return ans;
}

std::string Store::getValName() {
    return this->UserUseList[0]->getValueName();
}

std::string Store::getDstName() {
    return this->UserUseList[1]->getValueName();
}

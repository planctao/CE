//
// Created by me on 2023/10/31.
//

#include "IrUser.h"

IrUser::IrUser(int opNum) :IrValue(""){
    this->opNum = opNum;
}

void IrUser::addUserUseList(Use *use) {
    this->UserUseList.emplace_back(use);
}

std::vector<Use*> IrUser::getUserList() {
    return this->UserUseList;
}

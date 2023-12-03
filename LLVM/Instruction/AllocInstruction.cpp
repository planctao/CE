//
// Created by me on 2023/11/4.
//

#include "AllocInstruction.h"

#include <utility>

AllocInstruction::AllocInstruction(bool ifInit, IrValue *irValue,int dim1,int dim2) : Instruction() {
    this->ifInit = ifInit;
    this->irValue = irValue;
    this->dim1 = dim1;
    this->dim2 = dim2;
}

AllocInstruction::AllocInstruction(bool ifInit, std::string irName) {
    this->type = InstructionType::ALLOCA;
    this->ifInit = ifInit;
    this->opNum = 1;
    IrUser *thisUser = this;
    auto *tarValue = new IrValue(std::move(irName));
    Use* tarUse = new Use(tarValue,thisUser,0);
    tarValue->addUse(tarUse);
    //this->UserUseList(tarUse);
}

std::string AllocInstruction::irOutput() {
    std::string ans;
    return ans;
}



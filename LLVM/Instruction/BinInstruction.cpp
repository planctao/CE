//
// Created by me on 2023/10/31.
//

#include "BinInstruction.h"
#include "../ValueType/IntegerType.h"
BinInstruction::BinInstruction(IrValue *value1, IrValue *value2, InstructionType opType) {
    Use *use1 = new Use(value1,(IrUser*)this,0);
    Use *use2 = new Use(value2,(IrUser*)this,1);
    //value1->addUse(use1);
    //value2->addUse(use2);
    this->addUserUseList(use1);
    this->addUserUseList(use2);
    this->type = opType;
    this->valueType = new IntegerType(32);
}

/*注意:有i32和i1两种*/
std::string BinInstruction::irOutput() {
    std::string ans;
    int op = static_cast<int>(this->type);
    ans += this->name + " = " + instructionType2Str[op] + " " + "i32" + " " +
            this->UserUseList[0]->getValueName() + " , " + this->UserUseList[1]->getValueName();
    return ans;
}

std::string BinInstruction::getLeft() {
    return this->UserUseList[0]->getValueName();
}

std::string BinInstruction::getRight() {
    return this->UserUseList[1]->getValueName();
}

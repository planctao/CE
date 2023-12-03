//
// Created by me on 2023/11/12.
//

#include "GEP.h"

#include <utility>
#include "../Use.h"
#include "../ValueType/ValueType.h"
#include <iostream>
#include "../ValueType/PointerType.h"
#include "../ValueType/IntegerType.h"
GEP::GEP(ValueType* startType,IrValue *startValue,std::vector<int> tmpIndexList) {
    this->type = InstructionType::GETELEMENTPTR;
    this->startValueType = startType;
    Use *use = new Use(startValue,(IrUser*)this,0);
    this->addUserUseList(use);
    for (auto i : tmpIndexList) {
        this->indexList.emplace_back(i);
    }
    this->writeStandard = 0;
}

GEP::GEP(ValueType *startType, IrValue *startValue, std::vector<std::string> tmpOffsetList) {
    this->valueType = new PointerType(new IntegerType(32));
    this->startValueType = startType;
    this->type = InstructionType::GETELEMENTPTR;
    Use *use = new Use(startValue,(IrUser*)this,0);
    this->addUserUseList(use);
    this->offsetList = std::move(tmpOffsetList);
    this->writeStandard = 1;
}

std::string GEP::irOutput() {
    std::string ans;
    ans += this->name + " = ";
    ans += "getelementptr ";
    IrValue* value = this->UserUseList[0]->getIrValue();
    ans += startValueType->typeOutput() + ", " + value->getValueType()->typeOutput() + " " + value->getName() + ", ";
    for (int i = 0;i < this->offsetList.size();i++) {
        ans += "i32 " + offsetList.at(i);
        if (i < this->offsetList.size() - 1) {
            ans += ", ";
        }
    }
    return ans;
}

std::vector<std::string> GEP::getOffsetList() {
    return this->offsetList;
}



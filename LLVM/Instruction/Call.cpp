//
// Created by me on 2023/11/7.
//

#include "Call.h"

#include <utility>
#include "../ValueType/IntegerType.h"
Call::Call(IrValue* function, std::vector<IrValue *> paramList,int funcType) {
    this->type = InstructionType::CALL;
    this->funcType = funcType;
    Use* use = new Use(function,(IrUser*)this,0);
    this->addUserUseList(use);
    for (int i = 0;i < paramList.size();i++) {
        Use *tmpUse = new Use(paramList[i],(IrUser*)this,i+1);
        this->addUserUseList(tmpUse);
    }
    this->valueType = new IntegerType(32);
}

Call::Call(char ch,int funcType) {//putCh
    this->type = InstructionType::CALL;
    this->funcType = funcType;
    int num = ch;
    this->putName = std::to_string(num);
}

Call::Call(std::string numName, int funcType) {//putInt
    this->type = InstructionType::CALL;
    this->funcType = funcType;
    this->putName = std::move(numName);
}

Call::Call(int funcType) {//getint
    this->type = InstructionType::CALL;
    this->funcType = funcType;
    this->valueType = new IntegerType(32);

}

std::string Call::irOutput() {
    std::string ans;
    if (this->funcType == CALL_PUTCH) {
        ans += "call void @putch(i32 " + this->putName + ")";
        return ans;
    }
    else if (this->funcType == CALL_PUTINT) {
        ans += "call void @putint(i32 " + this->putName + ")";
        return ans;
    }
    else if (this->funcType == CALL_GETINT) {
        ans += this->name + "  = call i32 @getint() ";
        return ans;
    }

    Function *func = dynamic_cast<Function*>(this->UserUseList[0]->getIrValue());
    if(func->getReturnType() == "i32") {
        ans += this->name + " = ";
        ans += "call i32 " + this->UserUseList[0]->getValueName()+ "(";
    }
    else {
        ans += "call void " + this->UserUseList[0]->getValueName()+ "(";

    }
    for (int i = 1;i < this->UserUseList.size();i++) {
        ans += this->UserUseList[i]->getIrValue()->getValueType()->typeOutput() +
                " " + this->UserUseList[i]->getValueName();
        if (i < this->UserUseList.size() - 1) {
            ans += ",";
        }
    }
    ans += ")";
    return ans;
}

int Call::getFuncType() {
    return this->funcType;
}

std::string Call::getPutName() {
    return this->putName;
}

int Call::getParamNum() {
    return this->UserUseList.size() - 1;
}

std::string Call::getParamRName(int i) {
    return this->UserUseList[i]->getValueName();
}

std::string Call::getFuncLabel() {
    return this->UserUseList[0]->getValueName();
}









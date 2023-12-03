//
// Created by me on 2023/11/22.
//

#include "Icmp.h"
#include <iostream>
#include <utility>
Icmp::Icmp(IrValue *left, IrValue *right,int opType,std::string bitLen) {
    this->type = InstructionType::ICMP;
    this->valueType = new IntegerType(1);
    Use *useLeft = new Use(left,(IrUser*)this,0);
    Use *useRight = new Use(right,(IrUser*)this,1);
    this->UserUseList.emplace_back(useLeft);
    this->UserUseList.emplace_back(useRight);
    this->opType = opType;
    this->bitLen = std::move(bitLen);
}

Icmp::Icmp(IrValue *left, IrValue *right, std::string op,std::string bitLen) {
    this->type = InstructionType::ICMP;
    this->valueType = new IntegerType(1);
    Use *useLeft = new Use(left,(IrUser*)this,0);
    Use *useRight = new Use(right,(IrUser*)this,1);
    this->UserUseList.emplace_back(useLeft);
    this->UserUseList.emplace_back(useRight);
    if (op == "==") this->opType = ICMP_EQ;
    else if (op == "!=") this->opType = ICMP_NE;
    else if (op == ">") this->opType = ICMP_SGT;
    else if (op == "<") this->opType = ICMP_SLT;
    else if (op == ">=") this->opType = ICMP_SGE;
    else if (op == "<=") this->opType = ICMP_SLE;
    else {
        std::cout << "error in Icmp\n";
        this->opType = -1;
    }
    this->bitLen = std::move(bitLen);
}

std::string Icmp::irOutput() {
    std::string ans;
    ans += this->name + " = ";
    ans += "icmp ";
    switch (this->opType){
        case (ICMP_EQ): {
            ans += "eq";
            break;
        }
        case (ICMP_NE): {
            ans += "ne";
            break;
        }
        case (ICMP_SGT): {
            ans += "sgt";
            break;
        }
        case (ICMP_SLT): {
            ans += "slt";
            break;
        }
        case (ICMP_SGE): {
            ans += "sge";
            break;
        }
        case (ICMP_SLE): {
            ans += "sle";
            break;
        }
        default:{
            break;
        }
    }
    ans += " " + this->bitLen + " ";
    ans += this->UserUseList.at(0)->getValueName() + ", " + this->UserUseList.at(1)->getValueName();
    return ans;
}

int Icmp::getOpType() {
    return this->opType;
}

std::string Icmp::getLeft() {
    return this->UserUseList.at(0)->getValueName();
}

std::string Icmp::getRight() {
    return this->UserUseList.at(1)->getValueName();
}

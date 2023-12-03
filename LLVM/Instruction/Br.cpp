//
// Created by me on 2023/11/22.
//

#include "Br.h"
#include <iostream>
Br::Br(IrValue *judge, IrValue *label1, IrValue *label2) {
    this->type = InstructionType::BR;
    Use* use1 = new Use(judge,(IrUser*)this,0);
    Use* use2 = new Use(label1,(IrUser*)this,1);
    Use* use3 = new Use(label2,(IrUser*)this,2);
    this->addUserUseList(use1);
    this->addUserUseList(use2);
    this->addUserUseList(use3);
    this->name = "";
}

Br::Br(IrValue *label1) {
    this->type = InstructionType::BR;
    Use* use1 = new Use(label1,(IrUser*)this,0);
    this->addUserUseList(use1);
    this->name = "";
}

std::string Br::irOutput() {
    std::string ans;
    if (this->UserUseList.size() == 3) {
        ans += "br i1 " +  this->UserUseList.at(0)->getValueName() + ", label " +
                this->UserUseList.at(1)->getValueName()
                + ", label " + this->UserUseList.at(2)->getValueName();

        return ans;
    }
    else if (this->UserUseList.size() == 1) {

        ans += "br label " + this->UserUseList.at(0)->getValueName();
        return ans;
    }
    else {
        std::cout<< "br irOutput error\n";
        return "";
    }

}

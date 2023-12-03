//
// Created by me on 2023/11/18.
//

#include "Jal.h"

#include <utility>

Jal::Jal(std::string tarLabel) : MipsInstruction(MipsInstrType::JAL){
    this->label = std::move(tarLabel);
}

std::string Jal::mipsOutput() {
    std::string ans;
    ans += "jal " + this->label;
    return ans;
}

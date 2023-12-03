//
// Created by me on 2023/11/29.
//

#include "J.h"

#include <utility>

J::J(std::string label) : MipsInstruction(MipsInstrType::J){
    this->label = std::move(label);
}

std::string J::mipsOutput() {
    std::string ans;
    ans = "j " + this->label;
    return ans;
}



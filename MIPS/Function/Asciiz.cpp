//
// Created by me on 2023/11/14.
//

#include "Asciiz.h"

#include <utility>

Asciiz::Asciiz(std::string strName,std::string content) {
    this->strName = std::move(strName);
    this->content = std::move(content);
}

std::string Asciiz::mipsOutput() {
    std::string ans;
    ans += strName + ": .asciiz " + content;
    return ans;
}

//
// Created by me on 2023/11/29.
//

#ifndef COMPILER_J_H
#define COMPILER_J_H

#include "MipsInstruction.h"

class J : public MipsInstruction{
protected:
    std::string label;
public:
    explicit J(std::string label);
    std::string mipsOutput() override;
};


#endif //COMPILER_J_H

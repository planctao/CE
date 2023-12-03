//
// Created by me on 2023/11/18.
//

#ifndef COMPILER_JR_H
#define COMPILER_JR_H

#include "MipsInstruction.h"
#include <string>
class Jr : public MipsInstruction{
public:
    Jr();
    std::string mipsOutput();
};


#endif //COMPILER_JR_H

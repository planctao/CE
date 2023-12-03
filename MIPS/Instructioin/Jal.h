//
// Created by me on 2023/11/18.
//

#ifndef COMPILER_JAL_H
#define COMPILER_JAL_H

#include "MipsInstruction.h"
#include <string>
class Jal :public MipsInstruction{
protected:
    std::string label;
public:
    Jal(std::string tarLabel);
    std::string mipsOutput() override;
};


#endif //COMPILER_JAL_H

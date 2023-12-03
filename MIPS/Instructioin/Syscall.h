//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_SYSCALL_H
#define COMPILER_SYSCALL_H

#include "MipsInstruction.h"

class Syscall : public MipsInstruction{
public:
    Syscall();
    std::string mipsOutput() override;
};


#endif //COMPILER_SYSCALL_H

//
// Created by me on 2023/11/14.
//

#include "Syscall.h"

std::string Syscall::mipsOutput() {
    return "syscall";
}

Syscall::Syscall() : MipsInstruction(MipsInstrType::SYSCALL){

}

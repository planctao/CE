//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_INSTRUCTIONTYPE_H
#define COMPILER_INSTRUCTIONTYPE_H

#include <map>
enum class InstructionType{
    ADD,
    SUB,
    MUL,
    SDIV,
    ICMP,
    AND,
    OR,
    CALL,
    ALLOCA,
    LOAD,
    STORE,
    GETELEMENTPTR,
    PHI,
    ZEXT,
    TRUNC,
    BR,
    RET,
    MOD,
    NULLTYPE,
};
const std::string instructionType2Str[] = {"add","sub","mul","sdiv","icmp","and","or",
                                           "call","alloca","load","store","getelementptr",
                                           "phi","zext","trunc","br","ret","srem"};
#endif //COMPILER_INSTRUCTIONTYPE_H



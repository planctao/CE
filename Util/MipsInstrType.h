//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSINSTRTYPE_H
#define COMPILER_MIPSINSTRTYPE_H

#endif //COMPILER_MIPSINSTRTYPE_H
enum class MipsInstrType{
    ADD,
    ADDI,
    SUB,
    MUL,
    DIV,
    SW,
    LW,
    J,
    JAL,
    JR,
    LA,
    LI,
    MOVE,
    MFHI,
    SYSCALL,
    SLL,
    CMP,//这个是我自己创建的,包括seq,sne,sgt,sge,slt,sle六个,分别对应icmp里面的几个
    BEQ,
    BNE,
    NOP,
};
//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_BININSTRUCTION_H
#define COMPILER_BININSTRUCTION_H

#include <string>
#include "Instruction.h"
#include "../IrValue.h"
#include "../Use.h"
class BinInstruction : public Instruction{
public:
    BinInstruction(IrValue* value1, IrValue* value2,InstructionType opType);
    std::string irOutput() override;
    std::string getLeft();
    std::string getRight();
};


#endif //COMPILER_BININSTRUCTION_H

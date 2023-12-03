//
// Created by me on 2023/11/22.
//

#ifndef COMPILER_BR_H
#define COMPILER_BR_H

#include <string>
#include "../IrValue.h"
#include "Instruction.h"
#include "../Use.h"
class Br : public Instruction{
public:
    Br(IrValue* judge,IrValue *label1,IrValue *label2);
    explicit Br(IrValue *label1);
    std::string irOutput() override;
};


#endif //COMPILER_BR_H

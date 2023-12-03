//
// Created by me on 2023/11/7.
//

#ifndef COMPILER_LOAD_H
#define COMPILER_LOAD_H

#include "Instruction.h"
#include "../IrValue.h"
#include "../Use.h"
class Load : public Instruction{
public:
    explicit Load(IrValue *tar);
    std::string irOutput() override;

    std::string getSrc();
};


#endif //COMPILER_LOAD_H

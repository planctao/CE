//
// Created by me on 2023/11/23.
//

#ifndef COMPILER_TRUNC_H
#define COMPILER_TRUNC_H

#include <string>
#include "../IrValue.h"
#include "Instruction.h"
#include "../Use.h"
class Trunc : public Instruction{
public:
    explicit Trunc(IrValue* tar);
    std::string irOutput() override;
};


#endif //COMPILER_TRUNC_H

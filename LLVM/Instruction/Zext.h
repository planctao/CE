//
// Created by me on 2023/11/23.
//

#ifndef COMPILER_ZEXT_H
#define COMPILER_ZEXT_H

#include <string>
#include "../IrValue.h"
#include "Instruction.h"
#include "../Use.h"
class Zext : public Instruction{
public:
    explicit Zext(IrValue* tar);
    std::string irOutput() override;
};


#endif //COMPILER_ZEXT_H

//
// Created by me on 2023/11/11.
//

#ifndef COMPILER_RETURN_H
#define COMPILER_RETURN_H
#include "Instruction.h"
#include "../IrValue.h"
#include "../Use.h"

class Return : public Instruction{
public :
    Return(IrValue *ret);
    Return(std::string irName);
    std::string getRet();
    std::string irOutput() override;
};


#endif //COMPILER_RETURN_H

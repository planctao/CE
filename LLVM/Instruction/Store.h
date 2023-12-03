//
// Created by me on 2023/11/4.
//

#ifndef COMPILER_STORE_H
#define COMPILER_STORE_H

#include "../IrValue.h"
#include "../IrUser.h"
#include "../Use.h"
#include "Instruction.h"
#include <string>
class Store : public Instruction{
public:
    Store(std::string tarValue,std::string dest);
    Store(IrValue *val,IrValue *dst);
    std::string irOutput() override;
    std::string getValName();
    std::string getDstName();
};


#endif //COMPILER_STORE_H

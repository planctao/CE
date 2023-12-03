//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_USE_H
#define COMPILER_USE_H

#include "IrValue.h"
#include "IrUser.h"

class Use {
protected:
    int rank;
    IrValue *value;
    IrUser *user;

public:

    Use(IrValue *value, IrUser *user, int rank);
    std::string getValueName();
    IrValue *getIrValue();
};


#endif //COMPILER_USE_H

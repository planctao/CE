//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSINSTRUCTION_H
#define COMPILER_MIPSINSTRUCTION_H

#include "../../Util/MipsInstrType.h"
#include "../../Util/utils.h"
#include <iostream>
#include <string>
class MipsInstruction {
protected:
    MipsInstrType mipsInstrType;

public:
    MipsInstruction(MipsInstrType mipsInstrType);
    virtual std::string mipsOutput();
};


#endif //COMPILER_MIPSINSTRUCTION_H

//
// Created by me on 2023/11/17.
//

#ifndef COMPILER_MFHI_H
#define COMPILER_MFHI_H

#include "MipsInstruction.h"

class Mfhi : public MipsInstruction{
protected:
    int tarReg;
public:
    explicit Mfhi(int tarReg);
    std::string mipsOutput() override;
};


#endif //COMPILER_MFHI_H

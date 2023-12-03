//
// Created by me on 2023/11/17.
//

#ifndef COMPILER_MOVE_H
#define COMPILER_MOVE_H

#include "MipsInstruction.h"

class Move : public MipsInstruction{
protected:
    int dstReg;
    int srcReg;
public :
    Move(int dstReg,int srcReg);
    std::string mipsOutput() override;
};


#endif //COMPILER_MOVE_H

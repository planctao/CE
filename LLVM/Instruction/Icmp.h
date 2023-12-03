//
// Created by me on 2023/11/22.
//

#ifndef COMPILER_ICMP_H
#define COMPILER_ICMP_H

#include <string>
#include "Instruction.h"
#include "../IrValue.h"
#include "../Use.h"
#include "../ValueType/IntegerType.h"
#define ICMP_EQ 0
#define ICMP_NE 1
#define ICMP_SGT 2
#define ICMP_SGE 3
#define ICMP_SLT 4
#define ICMP_SLE 5
class Icmp : public Instruction{
protected:
    int opType;
    std::string bitLen;
public:
    Icmp(IrValue *left,IrValue *right,int opType,std::string bitLen);
    Icmp(IrValue *left,IrValue *right,std::string op,std::string bieLen);
    std::string getLeft();
    std::string getRight();
    std::string irOutput() override ;
    int getOpType();
};


#endif //COMPILER_ICMP_H

//
// Created by me on 2023/11/12.
//

#ifndef COMPILER_GEP_H
#define COMPILER_GEP_H

#include <vector>
#include "Instruction.h"
#include "../ValueType/ValueType.h"
class GEP : public Instruction{
protected:
    ValueType *startValueType;
    std::vector<int> indexList;//offset
    std::vector<std::string> offsetList;
    int writeStandard;
public:
    GEP(ValueType* startType,IrValue *startValue,std::vector<int> tmpIndexList);//writeStandard = 0
    GEP(ValueType* startType,IrValue *startValue,std::vector<std::string> tmpOffsetList);//writeStandard = 1
    std::string irOutput() override;
    std::vector<std::string> getOffsetList();
};


#endif //COMPILER_GEP_H

//
// Created by me on 2023/11/6.
//

#ifndef COMPILER_ARRAYTYPE_H
#define COMPILER_ARRAYTYPE_H

#include "ValueType.h"

class ArrayType : public ValueType{
protected:
    ValueType* elementType;
    int elementNum;
public:
    ArrayType(int elementNum,ValueType* type);
    std::string typeOutput() override;
    int getElementNum();
};


#endif //COMPILER_ARRAYTYPE_H

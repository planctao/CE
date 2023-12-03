//
// Created by me on 2023/11/6.
//

#ifndef COMPILER_INTEGERTYPE_H
#define COMPILER_INTEGERTYPE_H

#include "ValueType.h"

class IntegerType : public ValueType{
protected:
    int integerType;//i32 or i1
public:
    explicit IntegerType(int type);
    std::string typeOutput() override;
    int getIntegerType();
};


#endif //COMPILER_INTEGERTYPE_H

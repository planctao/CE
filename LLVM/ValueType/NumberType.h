//
// Created by me on 2023/11/7.
//

#ifndef COMPILER_NUMBERTYPE_H
#define COMPILER_NUMBERTYPE_H

#include "ValueType.h"

class NumberType : public ValueType{
public:
    NumberType();
    std::string typeOutput() override;
};


#endif //COMPILER_NUMBERTYPE_H

//
// Created by me on 2023/11/7.
//

#ifndef COMPILER_POINTERTYPE_H
#define COMPILER_POINTERTYPE_H

#include "ValueType.h"
#include <string>
class PointerType : public ValueType{
protected:
    ValueType* contained;
public:
    explicit PointerType(ValueType* contained);
    std::string typeOutput() override;
    IrValueType getContainedIrValueType();
    std::string containedTypeOutput();
    ValueType *getContained();
};


#endif //COMPILER_POINTERTYPE_H

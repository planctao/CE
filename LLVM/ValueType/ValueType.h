//
// Created by me on 2023/11/6.
//

#ifndef COMPILER_VALUETYPE_H
#define COMPILER_VALUETYPE_H

#include "../../Util/IrValueType.h"
#include <string>
class ValueType {
protected:
    IrValueType valueType;
public:
    ValueType(IrValueType type);
    virtual std::string typeOutput();
    IrValueType getIrValueType();
};


#endif //COMPILER_VALUETYPE_H

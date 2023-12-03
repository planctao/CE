//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_IRVALUE_H
#define COMPILER_IRVALUE_H

#include <string>
#include <vector>
#include "./ValueType/ValueType.h"
class Use;

class IrValue {
protected:
    ValueType *valueType;
    std::string name;
    std::vector<Use*> useList;
    bool ifTrue; //判断是否加了!这个符号
public:
    explicit IrValue(std::string name);
    IrValue(std::string name,ValueType *valueType);
    void addUse(Use *tmpUse);
    void setName(std::string tmpName);
    std::string getName();
    void setValueType(ValueType *valueType);
    ValueType *getValueType();
    virtual std::string irOutput();
    int getIntegerTypeValue();
    bool getIfTrue();
    void setIfTrue();
};


#endif //COMPILER_IRVALUE_H

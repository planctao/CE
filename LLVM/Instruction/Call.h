//
// Created by me on 2023/11/7.
//

#ifndef COMPILER_CALL_H
#define COMPILER_CALL_H

#include <vector>
#include <string>
#include "../../Util/utils.h"
#include "../Function/Function.h"
#include "../IrValue.h"
#include "../Use.h"
#define CALL_PRIMARY 0
#define CALL_PUTCH 1
#define CALL_PUTINT 2
#define CALL_GETINT 3
class Call : public Instruction{
    int funcType;//0:普通调用,1:
    std::string putName;
public:
    Call(IrValue* function,std::vector<IrValue*> paramList,int funcType);
    Call(char ch,int funcType); //putCh
    Call(std::string numName ,int funcType);//putint
    Call(int funcType);//getint
    int getFuncType();
    std::string getPutName();
    int getParamNum();
    std::string getParamRName(int i);
    std::string getFuncLabel();
    virtual std::string irOutput() override;
};


#endif //COMPILER_CALL_H

//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_IRUSER_H
#define COMPILER_IRUSER_H

#include <vector>
#include "IrValue.h"

class Use;

class IrUser : public IrValue{
protected:
    int opNum;
    std::vector<Use*> UserUseList;

public:
    IrUser(int opNum);
    void addUserUseList(Use*use);
    std::vector<Use*> getUserList();
};


#endif //COMPILER_IRUSER_H

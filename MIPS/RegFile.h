//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_REGFILE_H
#define COMPILER_REGFILE_H
#include<map>
#include<vector>
#include <stack>
//#include "MipsSymbolTable.h"
#include "MipsSymbol.h"
class MipsSymbolTable;
class RegFile {
protected:
    MipsSymbolTable *curMipsTable;
    std::map<int,MipsSymbol*> regSymbolMap;
    std::map<int,bool> regUseMap;
    std::stack<int> SRegUse;//s寄存器的使用情况

public:
    RegFile(MipsSymbolTable *curMipsTable);
    void setUse(int tar,MipsSymbol* tarSb);
    void setUse(int tar);
    void setFree(int tar);
    bool isV(int num);
    bool isS(int num);
    bool isA(int num);
    bool isT(int num);
    bool isRa(int num);

    int findFreeTReg();
    bool isUsed(int num);
};


#endif //COMPILER_REGFILE_H

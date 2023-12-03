//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSSYMBOLTABLE_H
#define COMPILER_MIPSSYMBOLTABLE_H

#include "RegFile.h"
#include <vector>
#include <map>
#include <string>
class MipsSymbol;
class MipsSymbolTable {
protected:
    std::string funcName;
    bool isMain;
    RegFile* regFile;//寄存器表
    std::map<std::string,MipsSymbol*> mipsSymbolList;
    int* fpTop;//从0开始，每次都-4
    int* gpTop;//从0开始，每次都-4
    int *spTop;
public:
    MipsSymbolTable(std::string funcName,bool isMain);
    MipsSymbol *findMipsSymbol(std::string name);
    void addMipsSymbol(MipsSymbol* mipsSymbol);
    int getFpTop();
    int getGpTop();
    int getSpTop();
    void addFp(int size);
    void addGp(int size);
    void addSp(int size);
    int findFreeReg();
    void setRegUse(int tar,MipsSymbol *sb);
    void setRegUse(int tarReg);
    void setFreeReg(int regIndex);
    void setSpTop(int sp);
    bool isUsed(int regIndex);
    bool ifMainFunc();
    std::string getFuncName();
    void eraseSb(std::string name);
};


#endif //COMPILER_MIPSSYMBOLTABLE_H

//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSSYMBOL_H
#define COMPILER_MIPSSYMBOL_H

#include <string>
class MipsSymbolTable;
class MipsSymbol {
protected:
    std::string mipsSymbolName; //指LLVM IR 中的%x的这个名字
    bool ifInReg;
    int regIndex;
    bool haveRam;
    int base;// gp=28, fp=30 标记当前符号在内存中的基地址所在的寄存器，具体地，全局变量是gp，局部变量是fp
    int offset;//相对于base的offset
    bool ifTemp;
    bool used;
    MipsSymbolTable *mipsSymbolTable;//存放在哪个函数中
    int sp;//偏移量，方便存取 没啥用，被offset替代
    bool storeAddress = false;
public:
    MipsSymbol(std::string name,MipsSymbolTable *mipsSymbolTable1,int sp);
    MipsSymbol(std::string name,MipsSymbolTable* mipsSymbolTable1);
    MipsSymbol(std::string name,int base,MipsSymbolTable *mipsSymbolTable1);//alloca
    MipsSymbol(std::string name,bool ifInReg,int regIndex,bool haveRam,int base,int offset,bool ifTemp,bool used,MipsSymbolTable* mipsSymbolTable1);//通用构造器
    MipsSymbol(std::string name,bool ifInReg,int regIndex,MipsSymbolTable* mipsSymbolTable1);//load
    std::string getName();
    void setMipsSymbolName(std::string mipsSymbolName);
    void setOffset(int offset);
    void setUsed(bool used);
    void setBase(int base);
    int getOffset();
    int getBase();
    int getRegIndex();
    bool getIfTemp();
    bool getIfUsed();
    bool getHaveRam();
    bool getIfInReg();
    bool isInReg();
    void isStoreAddress();
    bool getStoreAddress();
};


#endif //COMPILER_MIPSSYMBOL_H

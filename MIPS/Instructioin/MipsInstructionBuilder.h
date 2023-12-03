//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSINSTRUCTIONBUILDER_H
#define COMPILER_MIPSINSTRUCTIONBUILDER_H

#include "../../LLVM/Instruction/Instruction.h"
#include "../MipsSymbolTable.h"
#include "MipsInstruction.h"
class MipsInstructionBuilder {
protected:
    Instruction* irInstruction;
    MipsSymbolTable *mipsSymbolTable;
    std::vector<MipsInstruction*> mipsInstrList;
public:
    MipsInstructionBuilder(Instruction *irInstruction,MipsSymbolTable *mipsSymbolTable1);
    void genMipsInstr();
    void genAlloca();
    void genStore();
    void genLoad();
    void genAddSub();
    void genMul();
    void genDivMod();
    void genCall();
    void genRet();
    void genGetElementPtr();
    void genBr();
    void genIcmp();
    void genZext();
    std::vector<MipsInstruction*> getMipsInstrList();
    std::string irName();

    bool isNumberSymbol(std::string name);
    bool isGlobalSymbol(std::string name);
    bool isPartSymbol(std::string name);
    void fetchGEPOffset(bool ifPart,int tarReg,std::string srcName);
    void fetchValue(int tarReg,std::string srcName);
    void storeValueInStore(int srcReg,std::string dstName);
    void giveMemory(MipsSymbol* tarSb,int size);
    void addIntoMipsTable(MipsInstruction *mipsInstr);
    void freeReg(int regIndex);

    void setRegUse(int regIndex);
};


#endif //COMPILER_MIPSINSTRUCTIONBUILDER_H

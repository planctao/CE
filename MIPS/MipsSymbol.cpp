//
// Created by me on 2023/11/14.
//

#include "MipsSymbol.h"

#include <utility>


MipsSymbol::MipsSymbol(std::string name, MipsSymbolTable *mipsSymbolTable1, int sp) {
    this->mipsSymbolName = std::move(name);
    this->mipsSymbolTable = mipsSymbolTable1;
    this->sp = sp;
}

MipsSymbol::MipsSymbol(std::string name, MipsSymbolTable *mipsSymbolTable1) {
    this->mipsSymbolName = std::move(name);
    this->mipsSymbolTable = mipsSymbolTable1;
    this->sp = 99999;
}

/*给alloca用的*/
MipsSymbol::MipsSymbol(std::string name, int base,MipsSymbolTable *mipsSymbolTable1) {
    this->mipsSymbolName = std::move(name);
    this->base = base;
    this->regIndex = -1;//没有
    this->ifInReg = false;
    this->haveRam = false;
    this->offset = 0;
    this->ifTemp = false;
    this->used = false;
    this->mipsSymbolTable = mipsSymbolTable1;
    this->sp = 999999;
}

MipsSymbol::MipsSymbol(std::string name,bool ifInReg,int regIndex,bool haveRam,int base,int offset,bool ifTemp,bool used,MipsSymbolTable* mipsSymbolTable1) {
    this->mipsSymbolName = std::move(name);
    this->ifInReg = ifInReg;
    this->regIndex = regIndex;
    this->haveRam = haveRam;
    this->base = base;
    this->offset = offset;
    this->ifTemp = ifTemp;
    this->used = used;
    this->mipsSymbolTable = mipsSymbolTable1;
    this->sp = 99999999;
}

std::string MipsSymbol::getName() {
    return this->mipsSymbolName;
}

void MipsSymbol::setOffset(int tmpoffset) {
    this->offset = tmpoffset;
}

void MipsSymbol::setBase(int tmpbase) {
    this->base = tmpbase;
}

int MipsSymbol::getOffset() {
    return this->offset;
}

int MipsSymbol::getBase() {
    return this->base;
}

/*专门给load用的*/
MipsSymbol::MipsSymbol(std::string name, bool ifInReg, int regIndex, MipsSymbolTable *mipsSymbolTable1) {
    this->mipsSymbolName = std::move(name),
    this->ifInReg = ifInReg;
    this->regIndex = regIndex;
    this->used = false;
    this->ifTemp = true;
    this->mipsSymbolTable = mipsSymbolTable1;
    this->haveRam = false;

    /*以下没啥用*/
    this->base = -1;
    this->offset = -1;
    this->sp = 999999;
}

bool MipsSymbol::getIfTemp() {
    return this->ifTemp;
}

bool MipsSymbol::getIfUsed() {
    return this->used;
}

bool MipsSymbol::getHaveRam() {
    return this->haveRam;
}

bool MipsSymbol::getIfInReg() {
    return this->ifInReg;
}

int MipsSymbol::getRegIndex() {
    return this->regIndex;
}

void MipsSymbol::setUsed(bool u) {
    this->used = u;
}

bool MipsSymbol::isInReg() {
    return this->ifInReg;
}

void MipsSymbol::isStoreAddress() {
    this->storeAddress = true;
}

bool MipsSymbol::getStoreAddress() {
    return this->storeAddress;
}

void MipsSymbol::setMipsSymbolName(std::string name) {
    this->mipsSymbolName = std::move(name);
}

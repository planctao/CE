//
// Created by me on 2023/11/14.
//

#include "MipsSymbolTable.h"

#include <utility>

MipsSymbolTable::MipsSymbolTable(std::string funcName,bool isMain) {
    this->funcName = std::move(funcName);
    this->isMain = isMain;
    this->regFile = new RegFile(this);
    this->fpTop = new int(0);
    this->gpTop = new int(0);
    this->spTop = new int(0);
}

void MipsSymbolTable::addMipsSymbol(MipsSymbol *mipsSymbol) {
    std::string name = mipsSymbol->getName();
    this->mipsSymbolList.insert(std::make_pair(name,mipsSymbol));
}

int MipsSymbolTable::getFpTop() {
    return *this->fpTop;
}

int MipsSymbolTable::getGpTop() {
    return *this->gpTop;
}

int MipsSymbolTable::getSpTop() {
    return *this->spTop;
}

void MipsSymbolTable::addGp(int size) {
    *this->gpTop = *this->gpTop - size;
}

void MipsSymbolTable::addFp(int size) {
    *this->fpTop = *this->fpTop - size;
}

MipsSymbol *MipsSymbolTable::findMipsSymbol(std::string name) {
    return this->mipsSymbolList[name];
}

int MipsSymbolTable::findFreeReg() {
    return this->regFile->findFreeTReg();
}

void MipsSymbolTable::setRegUse(int tar,MipsSymbol* sb) {
    this->regFile->setUse(tar,sb);
}

void MipsSymbolTable::setFreeReg(int regIndex) {
    this->regFile->setFree(regIndex);
}

void MipsSymbolTable::setRegUse(int tarReg) {
    this->regFile->setUse(tarReg);
}

bool MipsSymbolTable::isUsed(int regIndex) {
    return this->regFile->isUsed(regIndex);
}

bool MipsSymbolTable::ifMainFunc() {
    return this->isMain;
}

void MipsSymbolTable::addSp(int size) {
    *this->spTop = *this->spTop - size;
}

void MipsSymbolTable::setSpTop(int sp) {
    *this->spTop = sp;
}

std::string MipsSymbolTable::getFuncName() {
    return this->funcName;
}

void MipsSymbolTable::eraseSb(std::string name) {
    this->mipsSymbolList.erase(name);
}

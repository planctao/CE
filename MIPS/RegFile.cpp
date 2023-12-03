//
// Created by me on 2023/11/14.
//

#include "RegFile.h"
#include <iostream>
RegFile::RegFile(MipsSymbolTable *curMipsTable) {
    this->curMipsTable = curMipsTable;
}

bool RegFile::isV(int num) {
    return num >= 2 && num <= 3;
}

bool RegFile::isS(int num) {
    return num >= 16 && num <= 23;
}

bool RegFile::isA(int num) {
    return num >= 4 && num <= 7;
}

bool RegFile::isT(int num) {
    return (num >= 8 && num <= 15) || (num >= 24 && num <= 25);
}

bool RegFile::isRa(int num) {
    return num == 31;
}

void RegFile::setUse(int tar,MipsSymbol *tarSb) {
    this->regUseMap[tar] = true;
    if (this->regSymbolMap.find(tar) == this->regSymbolMap.end()) {
        this->regSymbolMap.insert(std::make_pair(tar,tarSb));
    }
    else {
        std::cout << "setUse in RegFile is wrong!\n";
    }
}

void RegFile::setFree(int tar) {
    this->regUseMap[tar] = false;
    this->regSymbolMap.erase(tar);
}

int RegFile::findFreeTReg() {
    for (int i = 8;i <= 15;i++) {
        if (!regUseMap[i]) {
            return i;
        }
    }
    if (!regUseMap[24]) return 24;
    if (!regUseMap[25]) return 25;

    for(int i = 8;i <= 15;i++) {
        MipsSymbol *tmp = this->regSymbolMap[i];
        if (tmp!= nullptr && tmp->getIfTemp()) {
            int regIndex = tmp->getRegIndex();
            tmp->setUsed(true);
            this->regSymbolMap.erase(regIndex);
            this->regUseMap[regIndex] = false;
            return regIndex;
        }
    }
    std::cout << "can not find free reg\n";
    return -1;
}

void RegFile::setUse(int tar) {
    this->regUseMap[tar] = true;
}

bool RegFile::isUsed(int num) {
    return this->regUseMap[num];
}



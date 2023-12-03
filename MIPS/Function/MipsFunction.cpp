//
// Created by me on 2023/11/14.
//

#include "MipsFunction.h"
#include "../../LLVM/Function/Param.h"
#include "../BasicBlock/MipsBlockBuilder.h"
MipsFunction::MipsFunction(Function *irFunction) {
    this->irFunction = irFunction;
    std::string funcName = irFunction->getName();
    funcName.erase(0,1);
    this->funcName = funcName;
    this->ifMain = this->funcName == "main";
    this->mipsSymbolTable = new MipsSymbolTable(this->funcName,this->ifMain);
}

std::string MipsFunction::mipsOutput() {
    std::string ans;
    ans += this->funcName + " : " + "\n";
    for (auto block : this->blockList) {
        ans += block->mipsOutput();
    }
    return ans;
}

void MipsFunction::genMipsFunc() {
    std::vector<Param*> paramList = this->irFunction->getParamList();
    for (int i = 1;i <= paramList.size();i++) {
        Param *param = paramList[i - 1];
        if (i <= 3) {
            if (param->getDim() == 2 || param->getDim() == 3) {
                this->mipsSymbolTable->setRegUse(strReg2intReg("$a0") + i);
            }
            MipsSymbol *tmpSb = new MipsSymbol(param->getName(),true,strReg2intReg("$a0") + i,this->mipsSymbolTable);
            this->mipsSymbolTable->addMipsSymbol(tmpSb);
        }
        else {
            MipsSymbol *tmpSb = new MipsSymbol(param->getName(),false,-1,true,strReg2intReg("$sp"),-(i - 4) * 4,false,false,this->mipsSymbolTable);
            this->mipsSymbolTable->addMipsSymbol(tmpSb);
        }
    }
    int offset = (int)-(paramList.size() > 3 ? (paramList.size() - 3) * 4 : 0);
    this->mipsSymbolTable->setSpTop(offset);
    /*生成basicBlock*/
    std::vector<BasicBlock*> basicBlockList = this->irFunction->getBasicBlock();
    for (auto basicBlock : basicBlockList) {
        MipsBlockBuilder *tmpMipsBlockBuilder = new MipsBlockBuilder(this->irFunction,basicBlock,this->mipsSymbolTable);
        this->blockList.emplace_back(tmpMipsBlockBuilder->genBasicBlock());
    }
}

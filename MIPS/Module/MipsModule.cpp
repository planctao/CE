//
// Created by me on 2023/11/14.
//

#include "MipsModule.h"
#include "../../LLVM/Module/Module.h"
#include "../Function/Asciiz.h"
#include "../Function/Word.h"
#include "../Function/MipsFunction.h"
std::map<std::string,int> MipsModule::wordSizeMap;
std::vector<Asciiz*> MipsModule::asciizList;
MipsModule::MipsModule(Module *module) {
    this->irModule = module;
}

void MipsModule::genMipsModule() {
    genWordList();
    genFunctionList();
}

void MipsModule::genFunctionList() {
    for (auto irFunction : this->irModule->getFunctionList()) {
        MipsFunction *tmpMipsFunc = new MipsFunction(irFunction);
        tmpMipsFunc->genMipsFunc();
        this->functionList.emplace_back(tmpMipsFunc);
    }
}

void MipsModule::genWordList() {
    std::vector<GlobalVar*> globalVarList = this->irModule->getGlobalVarList();
    for (auto tmpGlobalVar : globalVarList) {
        std::vector<int> tmpArrayList = tmpGlobalVar->getArrayList();
        std::string varName = tmpGlobalVar->getName();
        varName.erase(0,1);//删掉前导@
        int dim = tmpGlobalVar->getDim();
        if (dim == 0) {
            MipsModule::wordSizeMap.insert(std::make_pair(varName,1));
            Word *word = new Word(tmpArrayList[0],varName);
            this->wordList.emplace_back(word);
        }
        else if (dim == 1 || dim == 2) {
            MipsModule::wordSizeMap.insert(std::make_pair(varName,tmpArrayList.size()));
            Word *word = new Word(tmpArrayList,varName);
            this->wordList.emplace_back(word);
        }

    }
}

void MipsModule::addAsciiz(Asciiz *asciiz) {
    MipsModule::asciizList.emplace_back(asciiz);
}

int MipsModule::getAsciizSize() {
    return MipsModule::asciizList.size();
}

std::string MipsModule::mipsOutput() {
    std::string ans;
    ans += ".data\n";
    for (auto word:this->wordList) {
        ans += word->mipsOutput() + "\n";
    }
    for (auto asciiz : MipsModule::asciizList) {
        ans += asciiz ->mipsOutput() + "\n";
    }
    ans += ".text\n";
    ans += "j main\n";

    for (auto f : this->functionList) {
        ans += f->mipsOutput() + "\n";
    }
    return ans;
}

int MipsModule::getWordSize(std::string wordName) {
    return MipsModule::wordSizeMap[wordName];
}


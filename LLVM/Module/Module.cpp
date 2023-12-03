//
// Created by me on 2023/10/31.
//

#include "Module.h"
#include "../Function/FunctionBuilder.h"
Module::Module(Node *curNode) : IrValue(""){
    this->curNode = curNode;
    this->curTable = new SymbolTable(nullptr);
}

void Module::genCompUnit() {
    for (auto& tmpNode : this->curNode->getChildren()) {
        FunctionBuilder* functionBuilder = new FunctionBuilder(this->curTable,&tmpNode);
        functionBuilder->gen();
        std::vector<GlobalVar*> varList = functionBuilder->getGlobalList();
        std::vector<Function*> funcList = functionBuilder->getFunctionList();
        this->globalVarList.insert(this->globalVarList.end(),varList.begin(),varList.end());
        this->functionList.insert(this->functionList.end(),funcList.begin(),funcList.end());
    }
}

std::string Module::irOutput() {
    std::string ans;
    for (auto tmpGlobalVar : this->globalVarList) {
        ans += tmpGlobalVar->irOutput() + "\n";
    }
    for (auto tmpFunction : this->functionList) {
        ans += tmpFunction->irOutput() + "\n";
    }
    return ans;
}

void Module::fileOutput(std::string ans) {
    std::ofstream fout("llvm_ir.txt");
    std::string declare = "declare i32 @getint()\n"
                          "declare void @putint(i32)\n"
                          "declare void @putch(i32)\n"
                          "declare void @putstr(i8*)\n";
    fout << declare;
    fout << ans;
    fout.close();
}

std::vector<Function *> Module::getFunctionList() {
    return this->functionList;
}

std::vector<GlobalVar *> Module::getGlobalVarList() {
    return this->globalVarList;
}

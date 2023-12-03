//
// Created by me on 2023/10/31.
//

#ifndef COMPILER_GLOBALVAR_H
#define COMPILER_GLOBALVAR_H

#include "../IrValue.h"
#include "../../GrammarTree/Node.h"
#include "../../Symbol/SymbolCon.h"
#include "../../Symbol/SymbolVar.h"
#include <string>
#include <vector>
class GlobalVar : public IrValue{
protected:
    Node *curNode;
    bool ifConst;
    int dim;
    std::vector<int> dimList;
    //std::vector<std::vector<int>> valueList;
    std::vector<int> arrayList;
public:
    GlobalVar(bool ifConst, std::string name, Node *curNode);
//    GlobalVar(bool ifConst, int dim, std::string varName,int single_value);//单变量
//    GlobalVar(bool ifConst, int dim, std::string varName,int size,std::vector<int> valueList);//一维数组
//    GlobalVar(bool ifConst, int dim, std::string varName,std::vector<int> dimList,std::vector<std::vector<int>> valueList);//二维数组
    void buildGlobalVar(SymbolTable *curTable);
    void buildSymbol(SymbolTable *curTable);
    std::vector<int> getArrayList();
    int getDim();
    std::string irOutput() override;
};


#endif //COMPILER_GLOBALVAR_H

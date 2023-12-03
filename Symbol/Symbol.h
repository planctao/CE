//
// Created by me on 2023/10/10.
//

#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H
#define VARIABLE_TYPE 0 //普通变量
#define ARRAY_1_TYPE 1 //一维数组的意思
#define ARRAY_2_TYPE 2 //二维数组的意思
#define FUNC_TYPE 3 //函数
#define CONST_KIND 0
#define VAR_KIND 1
#include<string>
#include<vector>
//#include "SymbolTable.h"
#include "../LLVM/IrValue.h"
#include "../LLVM/Instruction/Instruction.h"
class SymbolTable;
class Symbol {
protected:
    int id;
    int tableId;
    SymbolTable* curTable;
    std::string tokenName;
    int type; //变量/一维数组/二维数组/函数
    int kind; //const或者var

    /*数组的内容*/
    int dim;
    std::vector<int> dimList;
    std::vector<std::vector<int>> valueList;
    /*func的内容*/
    int returnType;
    int paramNum;
    std::vector<int> paraTypes;

    /*instruction 的内容*/
    IrValue *irValue;
public:
    Symbol()  = default;
    Symbol(int id,int tableId,std::string tokenValue,int type, int kind);
    Symbol(int tableId,std::string tokenName,int type,int kind,int dim,std::vector<int> dimList,std::vector<std::vector<int>> valueList);
    Symbol(int id,int tableId,std::string tokenName,int type, int returnType,int paramNum,std::vector<int> paraTypes);
    std::string getTokenValue();
    std::string getName();
    int getKind();
    int getSymbolType();
    int getReturnType();
    int getParaNum();
    int getDim();
    std::vector<int> getParaTypes();
    void setIrValue(IrValue *irValue);
    IrValue* getIrValue();
    std::vector<int> getDimList();
    void setDimList(int dim);
    void setDimList(std::vector<int> dimList);
    virtual int calInit() ;
    virtual int calInit(int x) ;
    virtual int calInit(int x,int y) ;
};


#endif //COMPILER_SYMBOL_H

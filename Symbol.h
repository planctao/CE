//
// Created by me on 2023/10/10.
//

#ifndef COMPILER_SYMBOL_H
#define COMPILER_SYMBOL_H
#define VARIABLE 0 //普通变量
#define ARRAY_1 1 //一维数组的意思
#define ARRAY_2 2 //二维数组的意思
#define FUNC 3 //函数
#define CONST 0
#define VAR 1
#include<string>
#include<vector>
class Symbol {
private:
    int id;
    int tableId;
    std::string tokenValue;
    int type; //变量/一维数组/二维数组/函数
    int kind; //const或者var


    /*func的内容*/
    int returnType;
    int paramNum;
    std::vector<int> paraTypes;

public:
    Symbol(int id,int tableId,std::string tokenValue,int kind);

};


#endif //COMPILER_SYMBOL_H

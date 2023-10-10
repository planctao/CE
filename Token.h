//
// Created by me on 2023/9/18.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include"TokenType.h"
#include<string>
#include<iostream>
#include<vector>
class Token {
private:
    TokenType type;
    int line;
    std::string value;

public:
    Token(TokenType type, int line, std::string value);
    void output();
    std::string formatOutput();
    std::string type2Str();
    std::string getValue();
    TokenType getType();
    void writeFile(std::vector<std::string>& a);
};



#endif //COMPILER_TOKEN_H

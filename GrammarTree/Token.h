//
// Created by me on 2023/9/18.
//

#ifndef COMPILER_TOKEN_H
#define COMPILER_TOKEN_H
#include"../Util/TokenType.h"
#include<string>
#include<iostream>
#include<vector>
class Token {
private:
    TokenType type;
    int line;
    std::string name;

public:
    Token(TokenType type, int line, std::string value);
    std::string output();
    std::string formatOutput();
    std::string type2Str();
    std::string getTokenName();
    TokenType getTokenType();
    TokenType getType();
    int getLine();
    void writeFile(std::vector<std::string>& a);
};



#endif //COMPILER_TOKEN_H

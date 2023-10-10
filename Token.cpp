//
// Created by me on 2023/9/29.
//

#include "Token.h"
#include<iostream>
#include<vector>
Token::Token(TokenType type, int line, std::string value) {
    this->type = type;
    this->line = line;
    this->value = std::move(value);//采用move函数后value的值会为空串，相当于资源的转移
}

void Token::output() {
    int numType = static_cast<int>(type);
    std::string typeString = typeStrings[numType];
    std::cout << typeString << " " << value << '\n';
}

void Token::writeFile(std::vector<std::string>& a) {
    int numType = static_cast<int>(type);
    std::string ans = typeStrings[numType] + " " + value;
    a.emplace_back(ans);
}

std::string Token::formatOutput() {
    int numType = static_cast<int>(type);
    std::string typeString(typeStrings[numType]);
    typeString.append(" ");typeString.append(this->value);
    return typeString;
}

std::string Token::type2Str() {
    int numType = static_cast<int>(type);
    return typeStrings[numType];
}

std::string Token::getValue() {
    return this->value;
}

TokenType Token::getType() {
    return this->type;
}


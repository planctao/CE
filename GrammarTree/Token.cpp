//
// Created by me on 2023/9/29.
//

#include "Token.h"
Token::Token(TokenType type, int line, std::string value) {
    this->type = type;
    this->line = line;
    this->name = std::move(value);//采用move函数后value的值会为空串，相当于资源的转移
}

std::string Token::output() {
    std::string ret;
    int numType = static_cast<int>(type);
    std::string typeString = typeStrings[numType];
    ret = typeString + " " + this->name + "\n";
    return ret;
    //std::cout << typeString << " " << this->name << '\n';
}

void Token::writeFile(std::vector<std::string>& a) {
    int numType = static_cast<int>(type);
    std::string ans = typeStrings[numType] + " " + this->name;
    a.emplace_back(ans);
}

std::string Token::formatOutput() {
    int numType = static_cast<int>(type);
    std::string typeString(typeStrings[numType]);
    typeString.append(" ");typeString.append(this->name);
    return typeString;
}

std::string Token::type2Str() {
    int numType = static_cast<int>(type);
    return typeStrings[numType];
}

std::string Token::getTokenName() {
    return this->name;
}

TokenType Token::getType() {
    return this->type;
}

int Token::getLine() {
    return this->line;
}

TokenType Token::getTokenType() {
    return this->type;
}


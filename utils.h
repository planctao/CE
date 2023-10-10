//
// Created by me on 2023/9/19.
//

#ifndef COMPILER_UTILS_H
#define COMPILER_UTILS_H
#include "TokenType.h"
#include "Token.h"
#include <string>
bool isNonDigit(char c);
bool isSingleSymbol(char c);
bool isCondSymbol(char c);

TokenType string2TokenType(std::string str);
#endif //COMPILER_UTILS_H

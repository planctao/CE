//
// Created by me on 2023/10/10.
//

#include "ErrorToken.h"
ErrorToken::ErrorToken(char type, int line, int pos) {
    this->type = type;
    this->pos = pos;
    this->line = line;
}

ErrorToken::ErrorToken(char type,int line) {
    this->type = type;
    this->line = line;
    this->pos = -1; //no mean
}

int ErrorToken::getErrorTokenLine() {
    return this->line;
}

char ErrorToken::getErrorTokenType() {
    return this->type;
}

std::string ErrorToken::getOutputString() {
    std::string ret;
    ret += std::to_string(this->line + 1);
    ret += " ";
    ret.push_back(this->type);
    ret += "\n";
    return ret;
}
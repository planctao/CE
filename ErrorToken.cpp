//
// Created by me on 2023/10/10.
//

#include "ErrorToken.h"
ErrorToken::ErrorToken(char type, int line, int pos) {
    this->type = type;
    this->pos = pos;
    this->line = line;
}
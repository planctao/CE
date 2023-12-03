//
// Created by me on 2023/10/31.
//

#include "Use.h"

Use::Use(IrValue *value, IrUser *user, int rank) {
    this->value = value;
    this->user = user;
    this->rank = rank;
}

std::string Use::getValueName() {
    return this->value->getName();
}

IrValue *Use::getIrValue() {
    return this->value;
}


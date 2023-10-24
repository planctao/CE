//
// Created by me on 2023/10/10.
//

#include "Error.h"
#include <utility>
Error::Error(Node node) : root{std::move(node)}{

}

void Error::check() {
    this->root.buildSymbolTable(0,-1);
}

void Error::output() {
    this->root.error_output();
}


void Error::establish_and_findError() {
    for (Node tmpNode : this->root.getChildren()) {

    }
}
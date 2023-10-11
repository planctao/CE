//
// Created by me on 2023/10/10.
//

#include "Error.h"
#include <utility>
Error::Error(Node node) : root{std::move(node)}{

}

void Error::check() {
    for(Node tmpNode : this->root.getChildren()) {
        if(tmpNode.getGrammarType() == GrammarType::ConstDecl) {
            if (tmpNode.getChildren().rbegin()->getToken().getValue() != ";") {

            }
        }
    }

}

void Error::output() {

}
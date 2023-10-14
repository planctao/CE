//
// Created by me on 2023/10/10.
//

#ifndef COMPILER_ERROR_H
#define COMPILER_ERROR_H
#include"../GrammarTree/Node.h"
#include"ErrorToken.h"
#include "../Util/GrammarType.h"
class Error {
private:
    Node root;
    std::vector<ErrorToken> errors;
public:
    explicit Error(Node node);
    void check();
    void output();
    void establish_and_findError();

};


#endif //COMPILER_ERROR_H

//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_ASCIIZ_H
#define COMPILER_ASCIIZ_H

#include <string>

class Asciiz {
protected:
    std::string strName;
    std::string content;

public:
    Asciiz(std::string strName,std::string content);
    std::string mipsOutput();
};


#endif //COMPILER_ASCIIZ_H

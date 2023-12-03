//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_WORD_H
#define COMPILER_WORD_H

#include <vector>
#include <string>
#define WORD_VAR 0
#define WORD_ARRAY1 1
#define WORD_ARRAY2 2
class Word {
protected:
    std::vector<int> init_array1;
    std::string wordName;
    int wordType;
public:
    Word(int init,std::string wordName);
    Word(std::vector<int> init_array,std::string wordName);
    std::string mipsOutput();
};


#endif //COMPILER_WORD_H

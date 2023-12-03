//
// Created by me on 2023/11/14.
//

#include "Word.h"

#include <utility>

Word::Word(int init, std::string wordName) {
    this->init_array1.emplace_back(init);
    this->wordName = std::move(wordName);
    this->wordType = WORD_VAR;
}

Word::Word(std::vector<int> init_array,std::string wordName) {
    //倒序装入数组
    for (int i = init_array.size()-1;i>=0;i--) {
        this->init_array1.emplace_back(init_array.at(i));
    }

    this->wordName = std::move(wordName);
    this->wordType = WORD_ARRAY1;
}



std::string Word::mipsOutput() {
    std::string ans;
    ans += wordName + ": .word ";

    for (auto i : this->init_array1) {
        ans += std::to_string(i) + " ,";
    }
    ans.erase(ans.size()-1,1);
    return ans;
}

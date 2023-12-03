//
// Created by me on 2023/11/14.
//

#ifndef COMPILER_MIPSMODULE_H
#define COMPILER_MIPSMODULE_H

#include <vector>
#include <string>
#include <map>
class Module;
class Word;
class Asciiz;
class MipsFunction;
class MipsModule {
protected:
    Module *irModule;
    std::vector<Word*> wordList;
    static std::vector<Asciiz*> asciizList;
    std::vector<MipsFunction*> functionList;
    static std::map<std::string,int> wordSizeMap;
public:
    explicit MipsModule(Module *module);
    void genMipsModule();
    void genFunctionList();
    static void addAsciiz(Asciiz* asciiz);
    static int getAsciizSize();
    void genWordList();
    std::string mipsOutput();
    static int getWordSize(std::string wordName);
};


#endif //COMPILER_MIPSMODULE_H

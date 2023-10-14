//
// Created by me on 2023/9/18.
//

#ifndef COMPILER_LEXER_H
#define COMPILER_LEXER_H

#include<string>
#include<fstream>
#include<iostream>
#include<vector>
#include<map>
#include"GrammarTree/Token.h"
#include"Util/TokenType.h"
#include"Util/utils.h"
class Lexer {
public:
    explicit Lexer(const char* path);
    explicit Lexer(std::string path);
    std::string getInputs();
    void run();
    void handleSingleSymbol(char c);
    int handleCondSymbol(int nowCursor);
    int handlePrintfSymbol(int nowCursor);
    void addToken(TokenType type,std::string value,int currentLine);
    void output();
    std::vector<Token> getTokens();
private:
    std::string inputs;
    //int cursor;//cursor指的是我们整个inputs里面的指针
    int line;//分析文本的第几行
    std::vector<Token> tokens;
    std::map<std::string,TokenType> reserveWordMap;//保留字
    std::map<std::string,TokenType> allWordMap;//所有字
    std::map<std::string,TokenType> condMap;//条件表达式
    TokenType reserve(std::string str);
    std::vector<std::string> wordValueList;
    std::vector<TokenType> wordTypeList;
};



#endif //COMPILER_LEXER_H

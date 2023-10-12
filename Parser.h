//
// Created by me on 2023/9/24.
//

#ifndef COMPILER_PARSER_H
#define COMPILER_PARSER_H

#include<iostream>
#include<vector>
#include<stack>
#include<fstream>
#include<string>
#include"Lexer.h"
#include"Token.h"
#include"Node.h"
#include"SymbolTable.h"
//#define DEBUG 1
//#define FUNC_DEBUG 1
//#define MAIN_DEBUG 1
class Parser {
private:
    Lexer lexer;
    std::vector<std::string> ans;
    int cursor;
    std::vector<Token> tokens;
    Node root;
    std::map<int,SymbolTable> symbolTableMap;
    std::ofstream fout;
public:
    void output();
    Node *getRoot();
    explicit Parser(const char *path);
    /*#####################*/
    Node parse();
    int parseDecl(int pos, Node* tmpNode);
    int parseStmt(int pos, Node* tmpNode);
    int parseFuncDef(int pos, Node* tmpNode);
    int parseFuncType(int pos, Node* tmpNode);
    int parseFuncFParams(int pos,Node* tmpNode);
    int parseBlock(int pos, Node* tmpNode);
    int parseBlockItem(int pos, Node* tmpNode);
    int parseMainFuncDef(int pos, Node* tmpNode);

    int parseConstDecl(int pos, Node* tmpNode);
    int parseVarDecl(int pos, Node* tmpNode);

    int parseConstDef(int pos, Node* tmpNode);
    int parseVarDef(int pos, Node* tmpNode);

    int parseConstExp(int pos, Node* tmpNode);
    int parseInitVal(int pos, Node* tmpNode);
    int parseConstInitVal(int pos, Node* tmpNode);

    int parseAddExp(int pos, Node* tmpNode);
    int parseMulExp(int pos, Node* tmpNode);
    int parseRelExp(int pos, Node* tmpNode);
    int parseEqExp(int pos, Node* tmpNode);
    int parseLOrExp(int pos, Node *tmpNode);
    int parseLAndExp(int pos, Node* tmpNode);

    int parseUnaryExp(int pos, Node* tmpNode);
    int parseFuncRParams(int pos, Node* tmpNode);
    int parseFuncFParam(int pos, Node* tmpNode);
    int parsePrimaryExp(int pos, Node* tmpNode);
    int parseExp(int pos, Node* tmpNode);

    int parseNumber(int pos, Node* tmpNode);
    int parseLVal(int pos, Node* tmpNode);

    int parseCond(int pos, Node* tmpNode);
    int parseForStmt(int pos,Node *tmpNode);

    int parseUnaryOp(int pos, Node *tmpNode);

    int parseBType(int pos, Node* tmpNode);

};


#endif //COMPILER_PARSER_H

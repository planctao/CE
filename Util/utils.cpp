//
// Created by me on 2023/9/19.
//

#include "utils.h"
#include "TokenType.h"
bool isNonDigit(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

bool isSingleSymbol(char c) {//判断是否是单符号，比如'+','-',',',各个括号之类的
    return c == '+' || c == '-' ||  c == '*' || c == '%' || c == ',' || c == ';' || c == '(' || c == ')' || c == '[' ||
           c == ']' || c == '{' || c == '}';
}

bool isCondSymbol(char c) {
    return c == '=' || c == '!' || c == '<' || c == '>';
}

TokenType string2TokenType(std::string str) {
#define strToType(s,type) \
    if (str == s) { \
        return TokenType::type;                \
    }

    strToType("main",MAINTK)
    strToType("const",CONSTTK)
    strToType("int",INTTK)
    strToType("break",BREAKTK)
    strToType("continue",CONTINUETK)
    strToType("if",IFTK)
    strToType("else",ELSETK)
    strToType("!",NOT)
    strToType("&&",AND)
    strToType("||",OR)
    strToType("for",FORTK)
    strToType("getint",GETINTTK)
    strToType("printf",PRINTFTK)
    strToType("return",RETURNTK)
    strToType("+",PLUS)
    strToType("-",MINU)
    strToType("void",VOIDTK)
    strToType("*",MULT)
    strToType("/",DIV)
    strToType("%",MOD)
    strToType("<",LSS)
    strToType("<=",LEQ)
    strToType(">",GRE)
    strToType(">=",GEQ)
    strToType("==",EQL)
    strToType("!=",NEQ)
    strToType("=",ASSIGN)
    strToType(";",SEMICN)
    strToType(",",COMMA)
    strToType("(",LPARENT)
    strToType("[",LBRACK)
    strToType("{",LBRACE)
    strToType(")",RPARENT)
    strToType("]",RBRACK)
    strToType("}",RBRACE)
    strToType("while",WHILETK)
    //接下来的是判断数字还是标识符
    if (isdigit(str[0])) {//如果合法只能是数字
        if(str[0] == '0') {
            if (str.size() > 1) {
                return TokenType::INVALIDTK;
            }
            else {
                return TokenType::INTCON;
            }
        }
        else {
            for (const auto & i : str) {
                if (!isdigit(i)) {}
                return TokenType ::INVALIDTK;
            }
            return TokenType::IDENFR;
        }
    }
    else if (isNonDigit(str[0])) {
        return TokenType::IDENFR;
    }

    return TokenType::UNKNOWED;
}

std::string GrammarType2String(GrammarType gt) {
    return grammarStrings[static_cast<int>(gt)];
}

std::string genArrayStmt(std::string typeName,std::vector<int> valueList){
    bool flag = true;//判断是不是全为0
    for (auto & i : valueList) {
        if (i != 0) {
            flag = false;
            break;
        }
    }
    if (flag) {
        return "zeroinitializer";
    }
    std::string ans = "[";
    for (int i = 0;i < valueList.size() - 1;i++) {
        ans += typeName + " " + std::to_string(valueList[i]) + ", ";
    }
    ans += typeName + " " + std::to_string(valueList[valueList.size() - 1]) + "]";
    return ans;
}

std::string genGlobalDef(bool if_const,std::string identName,int dim,std::vector<int> dimList,std::vector<std::vector<int>> valueList) {
    std::string ans = "@" + identName + " = dso_local ";
    if (if_const) {
        ans += "global ";
    }
    else {
        ans += "constant ";
    }
    if (dim == 0) {
        if (!valueList.empty() && !valueList[0].empty()) {
            ans += "i32 " + std::to_string(valueList[0][0]);
        }
        else {
            ans += "i32 0";
        }
    }
    else if (dim == 1) {
        ans += "[" + std::to_string(dimList[0]) + "* i32" + "] ";
        if (valueList.empty()) {
            ans += "zeroinitializer";
        }
        else {
            ans += genArrayStmt("i32",valueList[0]);
        }
    }
    else if (dim == 2) {
        ans += "[" + std::to_string(dimList[0]) + " ";
        if (valueList.empty()) {
            ans += "zeroinitializer";
        }
    }
    return ans;
}

int getFuncCnt(int *cnt) {
    int ret = *cnt;
    (*cnt) ++;
    return ret;
}

std::vector<std::vector<int>> zeroArray(int dim,std::vector<int>dimList) {
    std::vector<std::vector<int>>valueList;
    if (dim == 0) {
        valueList = {{0}};
    }
    else if (dim == 1) {
        std::vector<int> tmp(dimList[0],0);
        valueList.emplace_back(tmp);
    }
    else if (dim == 2) {
        for (int i = 0;i < dimList[0];i++) {
            std::vector<int> tmp(dimList[1],0);
            valueList.emplace_back(tmp);
        }
    }
    return valueList;
}

int str2int(std::string value) {
    int i = 0;
    for (auto ch : value) {
        i = i * 10 + ch - '0';
    }
    return i;
}


int strReg2intReg(std::string reg) {
    if (reg[0] == '$') {
        reg.erase(0,1);
    }
    if (reg == "fp") {return 30;}
    if (reg == "gp") {return 28;}
    if (reg == "sp") {return 29;}
    if (reg == "ra") {return 31;}
    if (reg == "at") {return 1;}
    if (reg == "zero") {return 0;}
    if (reg == "t8") {return 24;}
    if (reg == "t9") {return 25;}
    if (reg == "v0") {return 2;}
    if (reg == "v1") {return 3;}
    if (reg[0] == 't') { return 8 + reg[1] - '0';}
    if (reg[0] == 'a') {return 4 + reg[1] - '0';}
    if (reg[0] == 's') {return 16 + reg[1] - '0';}
    if (reg[0] == 'k') {return 26 + reg[1] - '0';}
    std::cout << "strReg2intReg error" << reg << "\n" ;
    return -1;
}

std::string intReg2strReg(int reg) {
    if (reg == 0) return "$zero";
    if (reg == 1) return "$at";
    if (reg == 30) return "$fp";
    if (reg == 29) return "$sp";
    if (reg == 28) return "$gp";
    if (reg == 31) return "$ra";
    if (reg == 24) return "$t8";
    if (reg == 25) return "$t9";
    if (reg >= 2 && reg <= 3) return "$v" + std::to_string(reg - 2);
    if (reg >= 4 && reg <= 7) return "$a" + std::to_string(reg - 4);
    if (reg >= 8 && reg <= 15)return "$t" + std::to_string(reg - 8);
    if (reg >= 16 && reg <= 23) return "$s" + std::to_string(reg - 16);
    if (reg >= 26 && reg <= 27) return "$k" + std::to_string(reg - 26);
    std::cout << "intReg2strReg error: the error reg == " << reg << "\n";
    return "";
}






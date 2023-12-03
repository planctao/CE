//
// Created by me on 2023/9/29.
//
#include"Lexer.h"
Lexer::Lexer(const char* path) {
    std::ifstream fin;
    fin.open(path);
    std::string str;
    /*char a;
    while(fin.get(a)){
        str.push_back(a);
    }*/
    std::getline(fin,str,'\0');
    this->inputs.append(str);
    //this->cursor = 0;
    this->line = 0;
    this->wordValueList = {""};
}

Lexer::Lexer(std::string path) {
    std::ifstream fin;

    fin.open(path);
    std::string str;
    /*char a;
    while(fin.get(a)){
        str.push_back(a);
    }*/
    std::getline(fin,str,'\0');
    this->inputs.append(str);
    //this->cursor = 0;
    this->line = 0;
}

std::string Lexer::getInputs() {
    return this->inputs;
}

void Lexer::addToken(TokenType type, std::string value, int currentLine) {
    Token tmpToken(type,currentLine,std::move(value));
    this->tokens.push_back(std::move(tmpToken));
}


void Lexer::handleSingleSymbol(char c) {
    std::string tmpValue;
    tmpValue.push_back(c);
    TokenType tmpType = string2TokenType(tmpValue);
    Token tmpToken(tmpType,this->line,std::move(tmpValue));
    this->tokens.push_back(std::move(tmpToken));
}

int Lexer::handleCondSymbol(int nowCursor) {//条件运算符比如 == ！= >= <=之类的
    int trueCursor = nowCursor;
    char nowCh = this->inputs[nowCursor];
    std::string tmpValue;
    tmpValue.push_back(nowCh);
    if (trueCursor + 1 < this->inputs.size() && this->inputs[trueCursor + 1] == '=') {
        tmpValue.push_back('=');
        trueCursor += 1;
    }

    TokenType tmpType = string2TokenType(tmpValue);
    addToken(tmpType,tmpValue,this->line);
    Token tmpToken(tmpType,this->line,tmpValue);
    return trueCursor + 1;
}

int Lexer::handlePrintfSymbol(int nowCursor) {
    int trueCursor = nowCursor;
    while(this->inputs[trueCursor] == ' ') {
        trueCursor++;
    }
    std::string nextSymbol;nextSymbol.push_back(this->inputs[trueCursor]);//读取下一个是不是'(';
    Lexer::addToken(string2TokenType(nextSymbol),nextSymbol,this->line);
    trueCursor++;
    while(this->inputs[trueCursor] == ' ') {
        trueCursor++;
    }
    if (this->inputs[trueCursor] == '"') {//遇到formatString
        std::string formatStringSymbol;
        formatStringSymbol.push_back('"');
        trueCursor++;
        while(this->inputs[trueCursor] != '"') {
            formatStringSymbol.push_back(this->inputs[trueCursor]);
            trueCursor++;
        }
        formatStringSymbol.push_back('"');
        addToken(TokenType::STRCON,formatStringSymbol,this->line);
        trueCursor++;
    }
    return trueCursor;
}


TokenType Lexer::reserve(std::string str) {//判断这个字符串是否是保留字
    TokenType tmpType = string2TokenType(str);
    if (tmpType == TokenType::UNKNOWED || tmpType == TokenType::INVALIDTK) {
        return tmpType;
    }
    addToken(tmpType,std::move(str),this->line);
    return tmpType;
}

void Lexer::run() {
    int cursor = 0;
    int size = this->inputs.size();
    while(cursor < size) {
        char nowCh = this->inputs[cursor];
        if (this->inputs[cursor] == ' ') { //空格直接跳过
            cursor++;
            continue;
        }
        else if (isNonDigit(nowCh)) {//如果是字符或者下划线
            std::string tmpValue;
            tmpValue.push_back(nowCh);
            cursor++;
            //读取到完全的字符
            while(cursor < size && (isNonDigit(this->inputs[cursor]) || isdigit(this->inputs[cursor]))) {
                tmpValue.push_back(this->inputs[cursor]);
                cursor++;
            }
            TokenType nowType = Lexer::reserve(tmpValue);
            if (nowType == TokenType::PRINTFTK) {
                cursor = handlePrintfSymbol(cursor);
            }
        }
        else if (isdigit(nowCh)) {//如果是数字
            std::string tmpValue;
            tmpValue.push_back(nowCh);
            cursor++;
            while(cursor < size && isdigit(this->inputs[cursor])) {
                tmpValue.push_back(this->inputs[cursor]);
                cursor++;
            }
            addToken(TokenType::INTCON,std::move(tmpValue),this->line);
        }
        else if (isSingleSymbol(nowCh)) {//单个字符的一些
            cursor++;
            Lexer::handleSingleSymbol(nowCh);
        }
        else if (nowCh == '/'){//如果是注释
            cursor++;
            char nextCh = this->inputs[cursor];
            if (nextCh == '/') {//如果是“//”那么直接读取到换行符为止
                while(cursor < size && this->inputs[cursor] != '\n') {
                    cursor++;
                }
                if (cursor < size) {
                    this->line++;
                    cursor++;
                }
            }
            else if (nextCh == '*') { //如果是“/*”
                cursor++;
                while(cursor + 1 < size && !(this->inputs[cursor] == '*' && this->inputs[cursor + 1] == '/')) {
                    if (this->inputs[cursor] == '\n') {
                        this->line++;
                    }
                    cursor++;
                }
                cursor += 2;
            }
            else {//正常的“/”
                Lexer::handleSingleSymbol('/');
            }
        }
        else if (isCondSymbol(nowCh)) {//条件运算符比如 == ！= >= <=之类的
            cursor = Lexer::handleCondSymbol(cursor);
        }
        else if (nowCh == '&' || nowCh == '|') {
            if (cursor + 1 < size) {
                char nextCh = this->inputs[cursor + 1];
                if (nextCh == nowCh) {
                    std::string tmpValue;tmpValue.push_back(nowCh);tmpValue.push_back(nextCh);
                    TokenType tmpType = string2TokenType(tmpValue);
                    addToken(tmpType,tmpValue,this->line);
                    cursor += 2;
                }
                else {
                    std::string tmpValue;tmpValue.push_back(nowCh);
                    addToken(TokenType::INVALIDTK,tmpValue,this->line);
                    cursor++;
                }
            }
            else {
                std::string tmpValue;tmpValue.push_back(nowCh);
                addToken(TokenType::INVALIDTK,tmpValue,this->line);
                cursor++;
            }
        }
        else if (nowCh == '\n') {
            this->line++;
            cursor++;
        }
        else {
            cursor++;
            continue;
        }
    }
}


void Lexer::output() {
    std::ofstream fout;
    fout.open("output.txt");
    for (auto& tmpToken: this->tokens) {
        fout << tmpToken.formatOutput() << " " << tmpToken.getLine() << std::endl;
    }
    fout.close();
}

std::vector<Token> Lexer::getTokens() {
    return this->tokens;
}

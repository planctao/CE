//
// Created by me on 2023/10/10.
//

#ifndef COMPILER_ERRORTOKEN_H
#define COMPILER_ERRORTOKEN_H


class ErrorToken {
private:
    int line;
    char type;
    int pos;
public:
    ErrorToken(char type,int line,int pos);
    ErrorToken(char type,int line);
};


#endif //COMPILER_ERRORTOKEN_H

cmake_minimum_required(VERSION 3.15)
project(compiler)

set(CMAKE_CXX_STANDARD 17)
#set(DCMAKE_SH="CMAKE_SH-NOTFOUND")


add_executable(compiler main.cpp Lexer.cpp Lexer.h TokenType.h Token.cpp Token.h utils.cpp utils.h Parser.cpp Parser.h Node.cpp Node.h GrammarType.h )
cmake_minimum_required(VERSION 3.15)
project(compiler)

set(CMAKE_CXX_STANDARD 17)
#set(DCMAKE_SH="CMAKE_SH-NOTFOUND")

#include_directories(${PROJECT_SOURCE_DIR}/Error)
#include_directories(${PROJECT_SOURCE_DIR}/GrammarTree)
#include_directories(${PROJECT_SOURCE_DIR}/Symbol)
#include_directories(${PROJECT_SOURCE_DIR}/Util)
#
#aux_source_directory(${PROJECT_SOURCE_DIR}/Error ERROR_SRC)
#aux_source_directory(${PROJECT_SOURCE_DIR}/GrammarTree GRAMMARTREE_SRC)
#aux_source_directory(${PROJECT_SOURCE_DIR}/Symbol SYMBOL_SRC)
#aux_source_directory(${PROJECT_SOURCE_DIR}/Util UTIL_SRC)
#
#add_executable(compiler main.cpp Lexer.cpp Lexer.h Parser.cpp Parser.h ${UTIL_SRC} ${SYMBOL_SRC} ${ERROR_SRC} ${GRAMMARTREE_SRC})
add_executable(compiler main.cpp Lexer.cpp Lexer.h Util/TokenType.h GrammarTree/Token.cpp GrammarTree/Token.h Util/utils.cpp Util/utils.h
        Parser.cpp Parser.h GrammarTree/Node.cpp GrammarTree/Node.h Util/GrammarType.h Symbol/Symbol.cpp Symbol/Symbol.h Symbol/SymbolTable.cpp Symbol/SymbolTable.h Error/Error.cpp Error/Error.h Error/ErrorToken.cpp Error/ErrorToken.h)
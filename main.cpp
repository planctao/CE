#include <iostream>
#include <fstream>
#include"Lexer.h"
#include"Parser.h"
#include"Error/Error.h"
#include"MidCode.h"
#include "./LLVM/Module/Module.h"
#include<vector>
#include "./MIPS/Module/MipsModule.h"
//#define MAINDEBUG 1
#define MODE 0
#define TEST 1
#define LEXER 2
#define PARSER 3
#define ERROR 4
#define LLVM 5
#define MIPS1 6
void fineError() {
    std::string folderPath("D:\\aaaaaaaaaaaa\\CE\\test\\20221105191751099\\full\\A");

    for (int i = 1;i <= 30;i++) {
        std::string testFilePath = folderPath + "\\testfile" + std::to_string(i) + ".txt";
        std::cout << testFilePath << '\n';
        Parser parser(testFilePath.data());
        Node tmpNode = parser.parse();
        parser.output();

        std::ifstream trueFile;
        std::string trueFileFolder = "D:\\aaaaaaaaaaaa\\CE\\test\\20221105191751099\\Syntaxanalysis\\A";
        std::string trueFilePath = trueFileFolder + "\\output" + std::to_string(i) + ".txt";
        trueFile.open(trueFilePath);
        std::string ansFilePath = "output.txt";
        std::ifstream ansFile;
        ansFile.open(ansFilePath);
        std::string trueContent;std::string ansContent;
        int line = 0;
        while(std::getline(trueFile,trueContent) && std::getline(ansFile,ansContent)) {
            //std::cout << "ok" << '\n';
            if (trueContent != ansContent) {
                //std::ofstream errorOut(errorFilePath);
                std::cout << "now the file is " << i << '\n';
                std::cout << "line == " << line << '\n' << "true == " << trueContent << '\n' << "yours == " << ansContent << '\n';
                //errorOut.close();
            }
            //std::cout << "true:" << trueContent << "   \n" << "your:" << ansContent << '\n';
            line++;
        }
        while(std::getline(ansFile,ansContent))
        ansFile.close();
        trueFile.close();
    }
}

int main(int argc, char* argv[]) {

    const char* input_path = "testfile.txt";
    int mode = MIPS1;
    switch(mode) {
        case TEST: {
            fineError();
            break;
        }


        case LEXER: {
            Lexer lexer(input_path);
            lexer.run();
            lexer.output();
            break;
        }


        case PARSER: {
            Parser parser(input_path);
            Node tmpNode = parser.parse();
            parser.output();
            break;
        }


        case ERROR: {
            Parser parser(input_path);
            Node tmpNode = parser.parse();
            Error error(std::move(tmpNode));
            error.check();
            error.output();
            break;
        }

        case LLVM: {
            Parser parser(input_path);
            Node tmpNode = parser.parse();
            Module *module = new Module(&tmpNode);
            module->genCompUnit();
            std::string ans = module->irOutput();
            module->fileOutput(ans);
            break;
        }

        case MIPS1: {
            Parser parser(input_path);
            Node tmpNode = parser.parse();
            Module *module = new Module(&tmpNode);
            module->genCompUnit();
            std::string llvmAns = module->irOutput();
            module->fileOutput(llvmAns);
            MipsModule *mipsModule = new MipsModule(module);
            mipsModule->genMipsModule();
            std::string ans = mipsModule->mipsOutput();
            std::ofstream fout("mips.txt");
            fout << ans;
            fout.close();
        }

        default:
            break;
    }
//#ifdef TEST
//    fineError();
//#else
//    std::cout << "start" << '\n';
//    Parser parser(input_path);
//    Node tmpNode = parser.parse();
//    Error error(std::move(tmpNode));
//    error.check();
//    error.output();
//
//
//    Lexer lexer(input_path);
//    lexer.run();
//    lexer.output();
//
//    Parser parser(intput_path);
//    Node tmpNode = parser.parse();
    return 0;
}

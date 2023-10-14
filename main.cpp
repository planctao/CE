#include <iostream>
#include"Lexer.h"
#include"Parser.h"
#include"Error/Error.h"
#include<vector>
//#define MAINDEBUG 1
//#define TEST 1
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

#ifdef TEST
    fineError();
#else
    Parser parser(input_path);
    Node tmpNode = parser.parse();
    Error error(std::move(tmpNode));
    error.check();
#endif
    return 0;
}

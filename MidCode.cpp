//
// Created by me on 2023/10/28.
//

#include "MidCode.h"
#include <utility>
int MidCode::curScope = GLOBAL;
MidCode::MidCode(Node root) : root{std::move(root)}{
    this->tableRoot = nullptr;
}

//
//void MidCode::visitCompUnit(Node *curNode) {
//    /*先建立当前的符号表*/
//    SymbolTable *newTable = new SymbolTable(0,nullptr,"Global");
//    this->tableRoot = newTable;
//    /*递归对语法树中的所有元素进行判断并且读取*/
//    for(auto &NodeChild : curNode->getChildren()) {
//        if (NodeChild.getGrammarType() == GrammarType::Decl) {
//            visitDecl(&NodeChild,newTable);
//        }
//        else if (NodeChild.getGrammarType() == GrammarType::FuncDef) {
//            visitFuncDef(&NodeChild,newTable);
//        }
//        else {
//            visitMainFuncDef(&NodeChild,newTable);
//        }
//    }
//}
//
//void MidCode::visitDecl(Node *curNode, SymbolTable *curTable) {
//    if (curNode->getChildren()[0].getGrammarType() == GrammarType::ConstDecl) {
//        visitConstDecl(&(curNode->getChildren()[0]),curTable);
//    }
//    else {
//        visitVarDecl(&(curNode->getChildren()[0]),curTable);
//    }
//}
//
//void MidCode::visitConstDecl(Node *curNode, SymbolTable *curTable) {
//    for (auto &child : curNode->getChildren()) {
//        if (child.getGrammarType() == GrammarType::ConstDef) {
//            visitConstDef(&child,curTable);
//        }
//    }
//}
//
//void MidCode::visitConstDef(Node* curNode, SymbolTable* curTable) {
//    int dim = curNode->calDim();
//    std::vector<int> dimList;
//    std::string identName = curNode->getChildren()[0].getLeafTokenName();
//    for (auto &child : curNode->getChildren()) {
//        if (child.getGrammarType() == GrammarType::ConstExp) {
//            int tmpDim = child.calConstExp(curTable);
//            dimList.emplace_back(tmpDim);
//        }
//    }
//    std::vector<int> valueList;
//    valueList = (curNode->getChildren()).back().calConstInitVal(curTable);
//    int type = (dim == 0) ? VARIABLE_TYPE :
//               (dim == 1) ? ARRAY_1_TYPE :
//               ARRAY_2_TYPE;
//    /*把需要的所有数据填入符号表中*/
//    Symbol tmpSb(curTable->getId(),identName,type,CONST_KIND,dim,dimList,valueList);
//    curTable->addSymbol(tmpSb);
//    /*根据上面获得的信息进行书写式子*/
////    if (curScope == GLOBAL) {
////        if (dim == 0) {
////            std::string st = "@" + identName + " = dso_local global i32 " + std::to_string(valueList[0][0]);
////            this->llvmIR.emplace_back(st);
////        }
////        else if (dim == 1) {
////            std::string st = "@" + identName + " = dso_local global ";
////            st += "[" + std::to_string(dimList[0]) + " i32] [";
////            for (int i = 0;i < valueList[0].size()-1;i++) {
////                st += "i32 " + std::to_string(valueList[0][i]) + ", ";
////            }
////            st += "i32 " + std::to_string(valueList[0][valueList.size()-1]) + "]";
////            this->llvmIR.emplace_back(st);
////        }
////        else {
////            std::string st =
////        }
////    }
//}
//
///*VarDecl → BType VarDef { ',' VarDef } ';'*/
//void MidCode::visitVarDecl(Node* curNode, SymbolTable* curTable) {
//    for (auto &child : curNode->getChildren()) {
//        if (child.getGrammarType() == GrammarType::VarDef) {
//            visitVarDef(&child,curTable);
//        }
//    }
//}
//
///*VarDef → Ident { '[' ConstExp ']' } | Ident { '[' ConstExp ']' } '=' InitVal*/
//void MidCode::visitVarDef(Node *curNode, SymbolTable *curTable) {
//    int dim = curNode->calDim();
//    std::vector<int> dimList;
//    std::vector<std::vector<int>> valueList;
//    /*求出每个constExp的值来填入到数组中*/
//    for (auto &child : curNode->getChildren()) {
//        if (child.getGrammarType() == GrammarType::ConstExp) {
//            dimList.emplace_back(child.calConstExp(curTable));
//        }
//    }
//    /*初始化valueList*/
//    if (dim == 1) {
//        std::vector<int> tmpList(dimList[0],0);
//        valueList.emplace_back(tmpList);
//    }
//    else if (dim == 2) {
//        int vecNum = dimList[0];
//        for (int i = 0;i < vecNum;i++) {
//            std::vector<int> tmpList(dimList[1],0);
//            valueList.emplace_back(tmpList);
//        }
//    }
//
//}
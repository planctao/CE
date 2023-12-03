//
// Created by me on 2023/11/4.
//

#ifndef COMPILER_INSTRUCTIONBUILDER_H
#define COMPILER_INSTRUCTIONBUILDER_H
#include <vector>
#include "../../Util/utils.h"
#include "../../GrammarTree/Node.h"

#include "../../Symbol/SymbolTable.h"
#include "../../Symbol/SymbolCon.h"
#include "../../Symbol/SymbolVar.h"


#include "../ValueType/IntegerType.h"
class InstructionBuilder {
protected:
    Node *curNode;
    int *cnt;
    SymbolTable *curTable;
    std::vector<Instruction*> instrList;

public:
    std::vector<Instruction*> getInstrList();
    InstructionBuilder(Node *curNode,SymbolTable* curTable, int* cnt);

    void genFuncBlock();
    void genInstr(Node *tmpNode);
    void genDecl(Node *tmpNode);
    void genConstDecl(Node *tmpNode);
    void genConstDef(Node *tmpNode);
    void genVarDecl(Node *tmpNode);
    void genVarDef(Node *tmpNode);
    void genInitVal(Node *tmpNode,IrValue *tarValue);
    void genStmt(Node* tmpNode);
    void genAssign(Node *tmpNode);
    void genReturn(Node *tmpNode);
    IrValue *genLVal(Node *tmpNode,bool ifLeft);
    IrValue *genExp(Node *tmpNode);
    IrValue *genAddExp(Node* tmpNode);
    IrValue *genMulExp(Node *tmpNode);
    IrValue *genUnaryExp(Node *tmpNode);
    IrValue *genPrimaryExp(Node *tmpNode);
    IrValue *genNumber(Node *tmpNode);
    std::vector<IrValue*> gemFuncRParams(Node *tmpNode);
    void genPrintf(Node *tmpNode);

    void genSetZero(IrValue *tarValue);
    void genParamInstr(std::string identName);
    void genArrayInitVal(int index,Node *tmpNode,IrValue *tarValue);
};


#endif //COMPILER_INSTRUCTIONBUILDER_H

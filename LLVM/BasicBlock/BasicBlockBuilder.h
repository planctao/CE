//
// Created by me on 2023/11/4.
//

#ifndef COMPILER_BASICBLOCKBUILDER_H
#define COMPILER_BASICBLOCKBUILDER_H

#include <vector>
#include <stack>
#include "BasicBlock.h"
#include "../Instruction/InstructionBuilder.h"
#include "../../Util/utils.h"
#include "../../Symbol/SymbolTable.h"
#include "../../GrammarTree/Node.h"
#include "../Instruction/Icmp.h"
#include "../Instruction/Br.h"
#include "../Instruction/Zext.h"
#include "../Instruction/Trunc.h"

class BasicBlockBuilder {
protected:
    SymbolTable* curTable;
    Node *curNode;
    int *cnt;
    int *blockCnt;
    std::vector<BasicBlock*> basicBlockList;
    static std::stack<BasicBlock*> breakBlock;
    static std::stack<BasicBlock*> continueBlock;
public:
    std::vector<BasicBlock*> getBasicBlock();
    BasicBlockBuilder(SymbolTable* curTable,Node* curNode,int *cnt);
    void buildBasicBlocks();
    void genIf(Node *curNode,BasicBlock **firstBlock,BasicBlock *nextBlock);
    void genCond(Node *curNode,BasicBlock *curBlock,BasicBlock *thenBlock,BasicBlock* elseBlock);
    void genLOrExp(Node *curNode,BasicBlock* curBlock,BasicBlock *thenBlock,BasicBlock* elseBlock);
    void genLAndExp(Node *curNode,BasicBlock* curBlock,BasicBlock *thenBlock,BasicBlock* elseBlock);
    IrValue* genEqExp(Node *curNode,BasicBlock* curBlock,BasicBlock *thenBlock,BasicBlock* elseBlock);
    IrValue* genRelExp(Node *curNode,BasicBlock* curBlock,BasicBlock *thenBlock,BasicBlock* elseBlock);

    void genFor(Node *curNode,BasicBlock **firstBlock,BasicBlock *nextBlock);
    void genForStmt(Node *curNode,BasicBlock *curBlock,BasicBlock *nextBlock);
    void genStmt(Node *curNode,BasicBlock **curBlock,BasicBlock*nextBlock,BasicBlock *followBlock);
    void genBlock(Node *curNode,BasicBlock **curBlock,BasicBlock *nextBlock,BasicBlock* followBlock);
    void genBlockItem(Node *curNode,BasicBlock **curBlock,BasicBlock *nextBlock,BasicBlock *followBlock);
    int getBlockCnt();
};


#endif //COMPILER_BASICBLOCKBUILDER_H

//
// Created by me on 2023/11/4.
//

#include "BasicBlockBuilder.h"
#include "../Instruction/Return.h"
std::stack<BasicBlock*> BasicBlockBuilder::breakBlock;
std::stack<BasicBlock*> BasicBlockBuilder::continueBlock;
BasicBlockBuilder::BasicBlockBuilder(SymbolTable *curTable, Node *curNode,int *cnt) {
    this->blockCnt = new int(0);
    this->curTable = curTable;
    this->curNode = curNode;
    this->cnt = cnt;
}

std::vector<BasicBlock *> BasicBlockBuilder::getBasicBlock() {
    return this->basicBlockList;
}

/*Block → '{' { BlockItem } '}'*/
void BasicBlockBuilder::buildBasicBlocks() {
    /*curNode = Block*/
    BasicBlock *firstBasicBlock = new BasicBlock("%block_"+std::to_string(getBlockCnt()), nullptr);
    Node blockNode = this->curNode->getChildren().back();
    InstructionBuilder* instrBuilder = new InstructionBuilder(&(blockNode.getChildren().at(0).getChildren()[0]),this->curTable,this->cnt);
    instrBuilder->genParamInstr("");
    firstBasicBlock->addInstruction(instrBuilder->getInstrList());
    this->basicBlockList.emplace_back(firstBasicBlock);
    genBlock(&blockNode,&firstBasicBlock,nullptr,nullptr);
    if (!this->curNode->returnInteger()) {
        Return *ret = new Return("void");
        firstBasicBlock->addInstruction(ret);
    }
}


/*Block → '{' { BlockItem } '}'*/
void BasicBlockBuilder::genBlock(Node *tmpNode, BasicBlock **curBlock,
        BasicBlock *nextBlock, BasicBlock *followBlock) {
    BasicBlock *nowBlock = *curBlock;
    for (int i = 1;i < tmpNode->getChildren().size() - 1;i++) {
        Node child = tmpNode->getChildren().at(i);
        genBlockItem(&child,&nowBlock,nullptr,nullptr);
//        if (isStmt(child.getChildren().at(0)) &&
//            (child.getChildren().at(0).getChildren().at(0).getLeafTokenName() == "for" ||
//             child.getChildren().at(0).getChildren().at(0).getLeafTokenName() == "if")) {
//            if (i != tmpNode->getChildren().size() - 2) {
//                BasicBlock* tmpNextBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
//                this->basicBlockList.emplace_back(tmpNextBlock);
//                genBlockItem(&child,&nowBlock,tmpNextBlock,followBlock);
//                nowBlock = tmpNextBlock;
//                *curBlock = nowBlock;
//            }
//            else {
//                if (nextBlock == nullptr) {
//                    BasicBlock *endBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
//                    this->basicBlockList.emplace_back(endBlock);
//                    Return *ret = new Return("void");
//                    endBlock->addInstruction(ret);
//                    genBlockItem(&child,&nowBlock,endBlock,followBlock);
//                }
//                else {
//                    genBlockItem(&child,&nowBlock,nextBlock,followBlock);
//                }
//            }
//        }
//        else {
//            genBlockItem(&child,&nowBlock,nullptr,nullptr);
//        }
    }
    *curBlock = nowBlock;
}

/*BlockItem → Decl | Stmt*/
void BasicBlockBuilder::genBlockItem(Node *tmpNode, BasicBlock **curBlock,
        BasicBlock *nextBlock, BasicBlock *followBlock) {
    for (auto child : tmpNode->getChildren()) {
        if (isDecl(child)) {
            InstructionBuilder tmpInstrBuilder(&child,this->curTable,this->cnt);
            tmpInstrBuilder.genDecl(&child);
            (*curBlock)->addInstruction(tmpInstrBuilder.getInstrList());
        }
        else if (isStmt(child)) {
            genStmt(&child,curBlock,nextBlock,followBlock);
        } else {std::cout << "error in genBlockItem\n";}
    }
}

/*
Stmt → LVal '=' Exp ';' // 每种类型的语句都要覆盖
| [Exp] ';' //有无Exp两种情况
| Block
| 'if' '(' Cond ')' Stmt [ 'else' Stmt ] // 1.有else 2.无else
| 'for' '(' [ForStmt] ';' [Cond] ';' [forStmt] ')' Stmt
| 'break' ';' | 'continue' ';'
| 'return' [Exp] ';' // 1.有Exp 2.无Exp
| LVal '=' 'getint''('')'';'
| 'printf''('FormatString{','Exp}')'';' // 1.有Exp 2.无Exp
*/
void BasicBlockBuilder::genStmt(Node *tmpNode, BasicBlock **curBlock, BasicBlock *nextBlock,BasicBlock *followBlock) {
    if (tmpNode->getChildren().at(0).getLeafTokenName() == "break") {
        Br *br = new Br(BasicBlockBuilder::breakBlock.top());
        (*curBlock)->addInstruction(br);
    }
    else if (tmpNode->getChildren().at(0).getLeafTokenName() == "continue") {
        Br *br = new Br(BasicBlockBuilder::continueBlock.top());
        (*curBlock)->addInstruction(br);
    }
    else if (tmpNode->getChildren().at(0).getGrammarType() == GrammarType::Block) {
        SymbolTable *nowTable = this->curTable;
        SymbolTable *newTable = new SymbolTable(nowTable);
        this->curTable = newTable;
        genBlock(&(tmpNode->getChildren().at(0)),curBlock,nextBlock,followBlock);
        this->curTable = nowTable;
    }
    else if (tmpNode->getChildren().at(0).getLeafTokenName() == "if") {
        genIf(tmpNode,curBlock,nextBlock);
    }
    else if (tmpNode->getChildren().at(0).getLeafTokenName() == "for") {
        genFor(tmpNode,curBlock,nextBlock);
    }
    else {
        InstructionBuilder tmpInstrBuilder(tmpNode,this->curTable,this->cnt);
        tmpInstrBuilder.genStmt(tmpNode);
        (*curBlock)->addInstruction(tmpInstrBuilder.getInstrList());
    }

}

/*'for' '(' [ForStmt] ';' [Cond] ';' [forStmt] ')' Stmt*/
void BasicBlockBuilder::genFor(Node *tmpNode, BasicBlock **firstBlock,BasicBlock *nextBlock) {
    BasicBlock *followBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
    this->basicBlockList.emplace_back(followBlock);
    BasicBlockBuilder::breakBlock.push(followBlock);
    BasicBlock *nowBlock = *firstBlock;
    BasicBlock *condBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
    BasicBlock *stmtBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
    BasicBlock *forStmt2Block = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
    this->basicBlockList.emplace_back(condBlock);
    this->basicBlockList.emplace_back(stmtBlock);
    this->basicBlockList.emplace_back(forStmt2Block);
    BasicBlockBuilder::continueBlock.push(forStmt2Block);
    if (tmpNode->forFindForStmt1() != -1) {
        Node forStmt1Node = tmpNode->getChildren().at(tmpNode->forFindForStmt1());
        genForStmt(&forStmt1Node,nowBlock,condBlock);//第一个ForStmt
    }
    else {
        Br *forStmt2Cond = new Br(condBlock);
        nowBlock->addInstruction(forStmt2Cond);
    }
    if (tmpNode->forFindCond() != -1) {
        Node condNode = tmpNode->getChildren().at(tmpNode->forFindCond());
        genCond(&condNode,condBlock,stmtBlock,followBlock);
    }
    else {
        Br *cond2Stmt = new Br(stmtBlock);
        condBlock->addInstruction(cond2Stmt);
    }
    Node stmtNode = tmpNode->getChildren().back();
    genStmt(&stmtNode,&stmtBlock,forStmt2Block,nullptr);
    Br *br = new Br(forStmt2Block);
    stmtBlock->addInstruction(br);
    if (tmpNode->forFindForStmt2() != -1) {
        Node forStmt2Node = tmpNode->getChildren().at(tmpNode->forFindForStmt2());
        genForStmt(&forStmt2Node,forStmt2Block,condBlock);
    }
    else {
        Br *forStmt2cond = new Br(condBlock);
        forStmt2Block->addInstruction(forStmt2cond);
    }
    *firstBlock = followBlock;
    BasicBlockBuilder::continueBlock.pop();
    BasicBlockBuilder::breakBlock.pop();
}

/*ForStmt → LVal '=' Exp*/
void BasicBlockBuilder::genForStmt(Node *tmpNode, BasicBlock *curBlock, BasicBlock *nextBlock) {
    Node lValNode = tmpNode->getChildren().at(0);
    InstructionBuilder tmpInstrBuilder(tmpNode,this->curTable,this->cnt);
    tmpInstrBuilder.genStmt(tmpNode);
    //tmpInstrBuilder.genLVal(&lValNode,true);
    curBlock->addInstruction(tmpInstrBuilder.getInstrList());
    Br *br = new Br(nextBlock);
    curBlock->addInstruction(br);
}

/*stmt → 'if' '(' Cond ')' Stmt [ 'else' Stmt ]*/
void BasicBlockBuilder::genIf(Node *tmpNode,BasicBlock **firstBlock,BasicBlock *nextBlock) {
    BasicBlock *nowBlock = *firstBlock;
    Node condNode = tmpNode->getChildren().at(2);
    Node thenNode = tmpNode->getChildren().at(4);
    BasicBlock *thenBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
    BasicBlock *followBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
    this->basicBlockList.emplace_back(thenBlock);
    this->basicBlockList.emplace_back(followBlock);
    if (tmpNode->getChildren().size() == 5) {
        genCond(&condNode,nowBlock,thenBlock,followBlock);
        genStmt(&thenNode,&thenBlock,nullptr,nullptr);
        Br *br = new Br(followBlock);
        thenBlock->addInstruction(br);
    }
    else if (tmpNode->getChildren().size() == 7) {
        Node elseNode = tmpNode->getChildren().at(6);
        BasicBlock *elseBlock = new BasicBlock("%block_" + std::to_string(getBlockCnt()));
        this->basicBlockList.emplace_back(elseBlock);
        genCond(&condNode,nowBlock,thenBlock,elseBlock);
        genStmt(&thenNode,&thenBlock,nextBlock,nullptr);
        genStmt(&elseNode,&elseBlock,nextBlock,nullptr);
        Br *br = new Br(followBlock);
        elseBlock->addInstruction(br);
        thenBlock->addInstruction(br);
    }
    *firstBlock = followBlock;
//    InstructionBuilder tmpInstr1(&thenNode,this->curTable,this->cnt);
//    tmpInstr1.genStmt(&thenNode);
//    thenBlock->addInstruction(tmpInstr1.getInstrList());
//    genStmt(&thenNode,&thenBlock,nextBlock,nullptr);
//    if (tmpNode->getChildren().size() == 7) {
//        Node elseNode = tmpNode->getChildren().at(6);
//        InstructionBuilder tmpInstr2(&elseNode,this->curTable,this->cnt);
//        tmpInstr2.genStmt(&elseNode);
//        elseBlock->addInstruction(tmpInstr2.getInstrList());
//        genStmt(&elseNode,&elseBlock,nextBlock,nullptr);
//        Br *br = new Br(nextBlock);
//        thenBlock->addInstruction(br);
//        elseBlock->addInstruction(br);
//    }
//    else if (tmpNode->getChildren().size() == 5) {
//        Br *br = new Br(nextBlock);
//        thenBlock->addInstruction(br);
//    }
}

void BasicBlockBuilder::genCond(Node *tmpNode, BasicBlock *curBlock, BasicBlock *thenBlock, BasicBlock *elseBlock) {
    Node lorNode= tmpNode->getChildren().at(0);
    genLOrExp(&lorNode,curBlock,thenBlock,elseBlock);
}

/*LOrExp → LAndExp | LOrExp '||' LAndExp*/
void BasicBlockBuilder::genLOrExp(Node *tmpNode,BasicBlock* curBlock,BasicBlock *thenBlock,BasicBlock* elseBlock) {
    if (tmpNode->getChildren().size() == 3) {
        Node lorNode = tmpNode->getChildren().at(0);
        Node lanNode = tmpNode->getChildren().at(2);
        BasicBlock *newBasicBlock = new BasicBlock("%block_"+std::to_string(getBlockCnt()));
        this->basicBlockList.emplace_back(newBasicBlock);
        genLOrExp(&lorNode,curBlock,thenBlock,newBasicBlock);
        genLAndExp(&lanNode,newBasicBlock,thenBlock,elseBlock);
    }
    else if (tmpNode->getChildren().size() == 1) {
        Node lanNode = tmpNode->getChildren().at(0);
        genLAndExp(&lanNode,curBlock,thenBlock,elseBlock);
    }
    else {
        std::cout << "error in genLorExp\n";
    }
}

/* LAndExp → EqExp | LAndExp '&&' EqExp*/
void BasicBlockBuilder::genLAndExp(Node *tmpNode,BasicBlock* curBlock,BasicBlock *thenBlock,BasicBlock* elseBlock) {
    if (tmpNode->getChildren().size() == 3) {
        Node landNode = tmpNode->getChildren().at(0);
        Node eqNode = tmpNode->getChildren().at(2);
        BasicBlock *newBasicBlock = new BasicBlock("%block_"+std::to_string(this->getBlockCnt()));
        this->basicBlockList.emplace_back(newBasicBlock);
        genLAndExp(&landNode,curBlock,newBasicBlock,elseBlock);
        IrValue *value = genEqExp(&eqNode,newBasicBlock,thenBlock,elseBlock);
        IrValue *zeroValue = new IrValue("0");
        int compare_icmp = value->getIfTrue() ? ICMP_NE : ICMP_EQ;
        if (value->getIntegerTypeValue() == 32) {
            Icmp *icmp = new Icmp(value,zeroValue,compare_icmp,"i32");
            icmp->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            newBasicBlock->addInstruction(icmp);
            Br *br = new Br(icmp,thenBlock,elseBlock);
            newBasicBlock->addInstruction(br);
        }
        else {
            Br *br = new Br(value,thenBlock,elseBlock);
            newBasicBlock->addInstruction(br);
        }

    }
    else if (tmpNode->getChildren().size() == 1) {
        Node eqNode = tmpNode->getChildren().at(0);
        IrValue *value = genEqExp(&eqNode,curBlock,thenBlock,elseBlock);
        if (value->getIntegerTypeValue() == 32) {
            IrValue *zeroValue = new IrValue("0");
            int compare_icmp = value->getIfTrue() ? ICMP_NE : ICMP_EQ;
            Icmp *icmp = new Icmp(value,zeroValue,ICMP_NE,"i32");
            icmp->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            curBlock->addInstruction(icmp);
            Br *br = new Br(icmp,thenBlock,elseBlock);
            curBlock->addInstruction(br);
        }
        else {
            Br *br = new Br(value,thenBlock,elseBlock);
            curBlock->addInstruction(br);
        }
    }
    else {
        std::cout << "error in genLandExp\n";
    }
}

/* EqExp → RelExp | EqExp ('==' | '!=') RelExp*/
IrValue* BasicBlockBuilder::genEqExp(Node *tmpNode, BasicBlock *curBlock, BasicBlock *thenBlock, BasicBlock *elseBlock) {
    if (tmpNode->getChildren().size() == 3) {
        Node eqNode = tmpNode->getChildren().at(0);
        Node relNode = tmpNode->getChildren().at(2);
        IrValue *left = genEqExp(&eqNode,curBlock,thenBlock,elseBlock);
        IrValue *right = genRelExp(&relNode,curBlock,thenBlock,elseBlock);
        int eq = tmpNode->getChildren().at(1).getLeafTokenName() == "==" ? ICMP_EQ : ICMP_NE;
        Icmp *icmp;
        if(left->getIntegerTypeValue() == 1 && right->getIntegerTypeValue() == 1) {
            icmp = new Icmp(left,right,eq,"i1");
        }
        else if (left->getIntegerTypeValue() == 1 && right->getIntegerTypeValue() == 32) {
            Zext *zext = new Zext(left);
            zext->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            curBlock->addInstruction(zext);
            icmp = new Icmp(zext,right,eq,"i32");
        }
        else if (left->getIntegerTypeValue() == 32 && right->getIntegerTypeValue() == 1) {
            Zext *zext = new Zext(right);
            zext->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            curBlock->addInstruction(zext);
            icmp = new Icmp(left,zext,eq,"i32");
        }
        else if (left->getIntegerTypeValue() == 32 && right->getIntegerTypeValue() == 32) {
            icmp = new Icmp(left,right,eq,"i32");
        } else {std::cout << "error in genEqExp\n";}
        curBlock->addInstruction(icmp);
        icmp->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        return icmp;
    }
    else if (tmpNode->getChildren().size() == 1) {
        Node relNode = tmpNode->getChildren().at(0);
        IrValue *rel = genRelExp(&relNode,curBlock,thenBlock,elseBlock);
        return rel;
    }
    else {
        std::cout << "error in genEqExp\n";
        return nullptr;
    }
}

/*RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp*/
IrValue* BasicBlockBuilder::genRelExp(Node *tmpNode, BasicBlock *curBlock, BasicBlock *thenBlock, BasicBlock *elseBlock) {
    if (tmpNode->getChildren().size() == 3) {
        Node relNode = tmpNode->getChildren().at(0);
        Node addNode = tmpNode->getChildren().at(2);
        IrValue* relValue = genRelExp(&relNode,curBlock,thenBlock,elseBlock);
        if (relValue->getIntegerTypeValue() == 1) {
            Zext *zext = new Zext(relValue);
            zext->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            curBlock->addInstruction(zext);
            relValue = zext;
        }
        InstructionBuilder tmpInstrBuilder(&addNode,this->curTable,this->cnt);
        IrValue *addValue = tmpInstrBuilder.genAddExp(&addNode);
        curBlock->addInstruction(tmpInstrBuilder.getInstrList());
        std::string op = tmpNode->getChildren().at(1).getLeafTokenName();
        Icmp *icmp = new Icmp(relValue,addValue,op,"i32");
        icmp->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        curBlock->addInstruction(icmp);
        return icmp;
    }
    else if (tmpNode->getChildren().size() == 1){
        Node addNode = tmpNode->getChildren().at(0);
        InstructionBuilder tmpInstrBuilder(&addNode,this->curTable,this->cnt);
        IrValue *addValue = tmpInstrBuilder.genAddExp(&addNode);
        curBlock->addInstruction(tmpInstrBuilder.getInstrList());

        if (!addValue->getIfTrue()) {
            IrValue *zeroValue = new IrValue("0");
            Icmp *icmp = new Icmp(addValue,zeroValue,ICMP_EQ,"i32");
            icmp->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            Zext *zext = new Zext(icmp);
            zext->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            curBlock->addInstruction(icmp);
            curBlock->addInstruction(zext);
            return zext;
        }
        else {
            return addValue;
        }
    }
    else {
        std::cout << "genRelExp error\n";
        return nullptr;
    }
}

int BasicBlockBuilder::getBlockCnt() {
    int ans = *this->blockCnt;
    *this->blockCnt = *this->blockCnt + 1;
    return ans;
}


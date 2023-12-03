//
// Created by me on 2023/11/14.
//
#include "../Module/MipsModule.h"
#include "../../Util/utils.h"
#include "MipsInstructionBuilder.h"
#include "../../LLVM/Instruction/BinInstruction.h"
#include "../../LLVM/Instruction/Store.h"
#include "../../LLVM/Instruction/Load.h"
#include "../../LLVM/Instruction/Call.h"
#include "../../LLVM/Instruction/Return.h"
#include "../../LLVM/Instruction/Alloc.h"
#include "../../LLVM/Instruction/GEP.h"
#include "../../LLVM/Instruction/Icmp.h"
#include "../../LLVM/Instruction/Br.h"
#include "../../LLVM/Instruction/Zext.h"

#include "Li.h"
#include "Lw.h"
#include "Sw.h"
#include "La.h"
#include "Move.h"
#include "Add.h"
#include "Addi.h"
#include "Sub.h"
#include "Mul.h"
#include "Div.h"
#include "Mfhi.h"
#include "Syscall.h"
#include "Jal.h"
#include "Jr.h"
#include "SLL.h"
#include "Cmp.h"
#include "Beq.h"
#include "Bne.h"
#include "J.h"
MipsInstructionBuilder::MipsInstructionBuilder(Instruction *irInstruction,MipsSymbolTable *mipsSymbolTable1) {
    this->irInstruction = irInstruction;
    this->mipsSymbolTable = mipsSymbolTable1;
}

std::vector<MipsInstruction *> MipsInstructionBuilder::getMipsInstrList() {
    return this->mipsInstrList;
}

void MipsInstructionBuilder::genMipsInstr() {
    if (this->irInstruction->getInstrType() == InstructionType::ALLOCA) {
        genAlloca();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::STORE) {
        genStore();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::LOAD) {
        genLoad();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::ADD ||
            this->irInstruction->getInstrType() == InstructionType::SUB) {
        genAddSub();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::MUL) {
        genMul();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::SDIV ||
            this->irInstruction->getInstrType() == InstructionType::MOD) {
        genDivMod();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::CALL) {
        genCall();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::RET) {
        genRet();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::GETELEMENTPTR) {
        genGetElementPtr();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::BR) {
        genBr();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::ICMP) {
        genIcmp();
    }
    else if (this->irInstruction->getInstrType() == InstructionType::ZEXT) {
        genZext();
    }
    else {
        std::cout << "error in MipsInstructionBuilder::genMipsInstr" << '\n';
    }
}

void MipsInstructionBuilder::genAlloca() {
    MipsSymbol *allocSymbol = new MipsSymbol(this->irName(),29,this->mipsSymbolTable);
    Alloc *alloc = dynamic_cast<Alloc*>(this->irInstruction);
    int size = alloc->getMemorySize();
    giveMemory(allocSymbol,size);
    this->mipsSymbolTable->addMipsSymbol(allocSymbol);
}

void MipsInstructionBuilder::genGetElementPtr() {
    std::vector<Use*> UserList = this->irInstruction->getUserList();
    GEP* gep = dynamic_cast<GEP*>(this->irInstruction);
    std::vector<std::string> offsetList = gep->getOffsetList();
    std::string baseName = UserList.at(0)->getValueName();
    MipsSymbol *gepSb = new MipsSymbol(this->irName(),29,this->mipsSymbolTable);
    gepSb->isStoreAddress();
    giveMemory(gepSb,4);
    this->mipsSymbolTable->addMipsSymbol(gepSb);
    if (offsetList.size() == 1) { //i32*
        MipsSymbol* baseSb = this->mipsSymbolTable->findMipsSymbol(UserList.at(0)->getValueName());
        int tmpReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(tmpReg);
        if (baseSb->getIfInReg()) {
            Move *move = new Move(tmpReg,baseSb->getRegIndex());
            addIntoMipsTable(move);
        }
        else {
            Lw *lw1 = new Lw(tmpReg,baseSb->getOffset(),baseSb->getBase());//地址
            addIntoMipsTable(lw1);
        }
        fetchGEPOffset(true,tmpReg,offsetList.at(0));
        Sw *sw = new Sw(tmpReg,gepSb->getOffset(),gepSb->getBase());
        addIntoMipsTable(sw);
        freeReg(tmpReg);
    }
    else if (offsetList.size() == 2){
        int tmpReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(tmpReg);
        if (isGlobalSymbol(baseName)) {
            baseName.erase(0,1);
            La *la = new La(tmpReg,baseName);
            addIntoMipsTable(la);
            int globalSize = MipsModule::getWordSize(baseName);
            if (globalSize > 1) {
                Addi *addi = new Addi(tmpReg,tmpReg,4*(globalSize-1));
                addIntoMipsTable(addi);
            }
        }
        else if (isPartSymbol(baseName)) {
            MipsSymbol* baseSb = this->mipsSymbolTable->findMipsSymbol(UserList.at(0)->getValueName());
            Addi *addi = new Addi(tmpReg,baseSb->getBase(),baseSb->getOffset());
            addIntoMipsTable(addi);
        }
        else {
            std::cout << baseName << '\n';
            std::cout << "error in MipsInstructionBuilder::genGetElementPtr::isGlobalSymbol()\n";
        }
        fetchGEPOffset(true,tmpReg,offsetList.at(1));
        Sw *sw = new Sw(tmpReg,gepSb->getOffset(),gepSb->getBase());//给gepSb里面存储的是地址
        addIntoMipsTable(sw);
        freeReg(tmpReg);
    }
    else {
        std::cout << "error in MipsInstructionBuilder::genGetElementPtr\n";
    }
}

/*获取gep的偏移量加到tarReg上,前提是tarReg存储的就是基地址*/
void MipsInstructionBuilder::fetchGEPOffset(bool ifPart,int tarReg,std::string srcName) {
    if (isGlobalSymbol(srcName)) {
        srcName.erase(0,1);
        int tmpReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(tmpReg);
        La *la = new La(tmpReg,srcName);addIntoMipsTable(la);
        Lw *lw = new Lw(tmpReg,0,tmpReg);addIntoMipsTable(lw);
        SLL *sll = new SLL(tmpReg,tmpReg,2);addIntoMipsTable(sll);
        if (ifPart) {
            Sub* sub = new Sub(tarReg,tarReg,tmpReg);addIntoMipsTable(sub);
        }
        else {
            Add *add = new Add(tarReg,tarReg,tmpReg);addIntoMipsTable(add);
        }
        freeReg(tmpReg);
    }
    else if (isPartSymbol(srcName)) {
        int tmpReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(tmpReg);
        MipsSymbol* srcSb = this->mipsSymbolTable->findMipsSymbol(srcName);
        Lw *lw = new Lw(tmpReg,srcSb->getOffset(),srcSb->getBase());
        addIntoMipsTable(lw);
        SLL *sll = new SLL(tmpReg,tmpReg,2);
        addIntoMipsTable(sll);
        if (ifPart) {
            Sub* sub = new Sub(tarReg,tarReg,tmpReg);addIntoMipsTable(sub);
        }
        else {
            Add *add = new Add(tarReg,tarReg,tmpReg);addIntoMipsTable(add);
        }
        freeReg(tmpReg);
    }
    else if (isNumberSymbol(srcName)) {
        int num = std::stoi(srcName);
        num = num * 4;
        if(ifPart) {
            num = -num;
        }
        Addi *addi = new Addi(tarReg,tarReg,num);
        addIntoMipsTable(addi);
    }
    else {
        std::cout << "error in MipsInstructionBuilder::fetchGEPOffset\n";
    }
}


/**
 *
 * 如果是单纯是一个数，那么用li.
 * 如果是变量，那么有两种，一种是全局变量，那么lw。如果是局部变量，看这个变量是不是还在Reg内，如果是则move，否则lw
 *
**/
void MipsInstructionBuilder::fetchValue(int srcReg,std::string srcName) {
    if (isNumberSymbol(srcName)) {
        int srcNum = std::stoi(srcName);
        Li *li = new Li(srcReg,srcNum);
        addIntoMipsTable(li);
    }
    else if (isGlobalSymbol(srcName)) {
        srcName.erase(0,1);
        La *la = new La(srcReg,srcName);
        addIntoMipsTable(la);
        Lw *lw = new Lw(srcReg,0,srcReg);
        addIntoMipsTable(lw);
    }
    else if (isPartSymbol(srcName)) {
        MipsSymbol* srcSb = this->mipsSymbolTable->findMipsSymbol(srcName);
        if (srcSb->getIfInReg()) {
            Move *move = new Move(srcReg,srcSb->getRegIndex());
            addIntoMipsTable(move);
            return;
        }
        if (srcSb->getStoreAddress()) {
            Lw* loadAddress = new Lw(srcReg,srcSb->getOffset(),srcSb->getBase());
            addIntoMipsTable(loadAddress);
            Lw *lw = new Lw(srcReg,0,srcReg);
            addIntoMipsTable(lw);
        }
        else{
            Lw* lw = new Lw(srcReg,srcSb->getOffset(),srcSb->getBase());
            addIntoMipsTable(lw);
        }
    }
    else {
        std::cout << "error in MipsInstructionBuilder::genStore\n";
        return;
    }
}

void MipsInstructionBuilder::storeValueInStore(int srcReg,std::string dstName) {
    int dstReg = this->mipsSymbolTable->findFreeReg();
    setRegUse(dstReg);
    if (isGlobalSymbol(dstName)) {
        std::string tmpDstName = dstName;
        tmpDstName.erase(0,1);
        La *la = new La(dstReg,tmpDstName);
        addIntoMipsTable(la);
        Sw *sw = new Sw(srcReg,0,dstReg);
        addIntoMipsTable(sw);
        freeReg(dstReg);
    }
    else if (isPartSymbol(dstName)) {
        MipsSymbol *dstSb = this->mipsSymbolTable->findMipsSymbol(dstName);
        if (dstSb->getStoreAddress()) {
            Lw *lw = new Lw(dstReg,dstSb->getOffset(),dstSb->getBase());
            addIntoMipsTable(lw);
            Sw *sw = new Sw(srcReg,0,dstReg);
            addIntoMipsTable(sw);
        }
        else{
            Sw *sw = new Sw(srcReg,dstSb->getOffset(),dstSb->getBase());
            addIntoMipsTable(sw);
        }
        freeReg(dstReg);
    }
    else {
        std::cout << "error in MipsInstructionBuilder::storeValueInStore\n";
        return;
    }
}

void MipsInstructionBuilder::genStore() {
    Store *storeInstr = dynamic_cast<Store*>(this->irInstruction);
    std::string srcName = storeInstr->getValName();
    std::string dstName = storeInstr->getDstName();
    int srcReg = mipsSymbolTable->findFreeReg();
    setRegUse(srcReg);
    /*先获取src的值存到srcReg*/
    fetchValue(srcReg,srcName);
    /*将srcReg里面的值存到dst内*/
    storeValueInStore(srcReg,dstName);
    freeReg(srcReg);
}

void MipsInstructionBuilder::genLoad() {
    Load *load = dynamic_cast<Load*>(this->irInstruction);
    std::string loadName = load->getName();
    MipsSymbol *mipsSymbol = new MipsSymbol(loadName,29,this->mipsSymbolTable);
    giveMemory(mipsSymbol,4);
    this->mipsSymbolTable->addMipsSymbol(mipsSymbol);
    std::string srcName = load->getSrc();
    if (isPartSymbol(srcName)) {
        MipsSymbol* srcMipsSb = this->mipsSymbolTable->findMipsSymbol(srcName);
        int tmpReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(tmpReg);
        if (srcMipsSb->isInReg()) {//如果直接在寄存器中，直接取出值来
            Move *move = new Move(tmpReg,srcMipsSb->getRegIndex());
            this->mipsInstrList.emplace_back(move);
            Sw *sw = new Sw(tmpReg,mipsSymbol->getOffset(),mipsSymbol->getBase());
            this->mipsInstrList.emplace_back(sw);
            freeReg(tmpReg);
            return;
        }
        /*要从内存中取出我们想要的值*/
        if (srcMipsSb->getStoreAddress()) {//如果srcMipsSb里面存的是地址，要先把这个地址lw出来，然后再来一次lw才是真正得到想要的那个值
            Lw *addressLw = new Lw(tmpReg,srcMipsSb->getOffset(),srcMipsSb->getBase());
            this->mipsInstrList.emplace_back(addressLw);
            Lw *lw = new Lw(tmpReg,0,tmpReg);
            this->mipsInstrList.emplace_back(lw);
        }
        else {//如果srcMipsSb里面存的是值，只需要我们把值lw出来即可
            Lw * lw = new Lw(tmpReg,srcMipsSb->getOffset(),srcMipsSb->getBase());
            this->mipsInstrList.emplace_back(lw);
        }
        Sw *sw = new Sw(tmpReg,mipsSymbol->getOffset(),mipsSymbol->getBase());
        this->mipsInstrList.emplace_back(sw);
        freeReg(tmpReg);
    }
    else if (isGlobalSymbol(srcName)) {
        int tmpReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(tmpReg);
        srcName.erase(0,1);//去除前导@
        La *la = new La(tmpReg,srcName);
        this->mipsInstrList.emplace_back(la);
        Lw *lw = new Lw(tmpReg,0,tmpReg);
        this->mipsInstrList.emplace_back(lw);
        Sw *sw = new Sw(tmpReg,mipsSymbol->getOffset(),mipsSymbol->getBase());
        this->mipsInstrList.emplace_back(sw);
        freeReg(tmpReg);
    }


    /*先不用这个，下次优化再说*/
//    if (freeReg > 0) {
//        Lw *lw = new Lw(freeReg,srcMipsSb->getOffset(),srcMipsSb->getBase());
//        MipsSymbol *dstMipsSb = new MipsSymbol(loadName,true,freeReg,this->mipsSymbolTable);
//        this->mipsSymbolTable->addMipsSymbol(dstMipsSb);
//        this->mipsSymbolTable->setRegUse(freeReg,dstMipsSb);
//    }
//    else {
//        /*TODO*/
//    }
}

/**
 * 这么几种情况：
 * 1.两个局部变量：lw，lw，add,sw
 * 2.两个都是全局变量：la,lw,la,lw,add,sw
 * 3.一个全局一个局部：la,lw,lw,add,sw
 * 4.一个全局一个数：la,lw,addi,sw
 * 5.一个局部一个数:lw,addi,sw
 * 6.两个数：li,sw
 */
void MipsInstructionBuilder::genAddSub() {
    BinInstruction* bin = dynamic_cast<BinInstruction*>(this->irInstruction);
    std::string leftName = bin->getLeft();
    std::string rightName = bin->getRight();
    MipsSymbol* leftSb;
    MipsSymbol* rightSb;
    int leftReg = -1,rightReg = -1;
    //左边
    if (isGlobalSymbol(leftName)) {
        leftSb = this->mipsSymbolTable->findMipsSymbol(leftName);
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        La *la = new La(leftReg,leftName);
        Lw *lw = new Lw(leftReg,leftSb->getOffset(),leftSb->getBase());
        addIntoMipsTable(la);
        addIntoMipsTable(lw);
    }
    else if (isPartSymbol(leftName)) {
        leftSb = this->mipsSymbolTable->findMipsSymbol(leftName);
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        Lw *lw = new Lw(leftReg,leftSb->getOffset(),leftSb->getBase());
        addIntoMipsTable(lw);
    }
    else {
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        Li *li = new Li(leftReg,std::stoi(leftName));
        this->mipsInstrList.emplace_back(li);
    }
    //右边
    if (isGlobalSymbol(rightName)) {
        rightSb = this->mipsSymbolTable->findMipsSymbol(rightName);
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        La *la = new La(rightReg,rightName);
        Lw *lw = new Lw(rightReg,rightSb->getOffset(),rightSb->getBase());
        addIntoMipsTable(la);
        addIntoMipsTable(lw);
    }
    else if (isPartSymbol(rightName)) {
        rightSb = this->mipsSymbolTable->findMipsSymbol(rightName);
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        Lw *lw = new Lw(rightReg,rightSb->getOffset(),rightSb->getBase());
        addIntoMipsTable(lw);
    }
    else {
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        Li *li = new Li(rightReg,std::stoi(rightName));
        this->mipsInstrList.emplace_back(li);

    }
    int dstReg = this->mipsSymbolTable->findFreeReg();
    setRegUse(dstReg);
    if (bin->getInstrType() == InstructionType::ADD) {
        Add *add = new Add(dstReg, leftReg, rightReg);
        this->mipsInstrList.emplace_back(add);
    } else {
        Sub *sub = new Sub(dstReg, leftReg, rightReg);
        this->mipsInstrList.emplace_back(sub);
    }
    MipsSymbol *resultSb = new MipsSymbol(bin->getName(), 29, this->mipsSymbolTable);
    giveMemory(resultSb, 4);
    Sw *sw = new Sw(dstReg, resultSb->getOffset(), resultSb->getBase());
    this->mipsInstrList.emplace_back(sw);
    this->mipsSymbolTable->addMipsSymbol(resultSb);
    freeReg(dstReg);
    if (leftReg > 0) freeReg(leftReg);
    if (rightReg > 0) freeReg(rightReg);

}

void MipsInstructionBuilder::genMul() {
    BinInstruction* bin = dynamic_cast<BinInstruction*>(this->irInstruction);
    std::string leftName = bin->getLeft();
    std::string rightName = bin->getRight();
    MipsSymbol* leftSb;
    MipsSymbol* rightSb;
    int leftReg = -1,rightReg = -1;
    if (isGlobalSymbol(leftName)) {
        leftSb = this->mipsSymbolTable->findMipsSymbol(leftName);
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        La *la = new La(leftReg,leftName);
        Lw *lw = new Lw(leftReg,leftSb->getOffset(),leftSb->getBase());
        addIntoMipsTable(la);
        addIntoMipsTable(lw);
    }
    else if (isPartSymbol(leftName)) {
        leftSb = this->mipsSymbolTable->findMipsSymbol(leftName);
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        Lw *lw = new Lw(leftReg,leftSb->getOffset(),leftSb->getBase());
        addIntoMipsTable(lw);

    }
    else {
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        Li *li = new Li(leftReg,std::stoi(leftName));
        this->mipsInstrList.emplace_back(li);
    }
    if (isGlobalSymbol(rightName)) {
        rightSb = this->mipsSymbolTable->findMipsSymbol(rightName);
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        La *la = new La(rightReg,rightName);
        Lw *lw = new Lw(rightReg,rightSb->getOffset(),rightSb->getBase());
        addIntoMipsTable(la);
        addIntoMipsTable(lw);
    }
    else if (isPartSymbol(rightName)) {
        rightSb = this->mipsSymbolTable->findMipsSymbol(rightName);
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        Lw *lw = new Lw(rightReg,rightSb->getOffset(),rightSb->getBase());
        addIntoMipsTable(lw);
    }
    else {
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        Li *li = new Li(rightReg,std::stoi(rightName));
        this->mipsInstrList.emplace_back(li);
    }
    int dstReg = this->mipsSymbolTable->findFreeReg();
    setRegUse(dstReg);
    Mul *mul = new Mul(dstReg,leftReg,rightReg);
    this->mipsInstrList.emplace_back(mul);
    MipsSymbol *dstSb = new MipsSymbol(bin->getName(),29,this->mipsSymbolTable);
    giveMemory(dstSb,4);
    this->mipsSymbolTable->addMipsSymbol(dstSb);
    Sw *sw = new Sw(dstReg,dstSb->getOffset(),dstSb->getBase());
    this->mipsInstrList.emplace_back(sw);
    freeReg(leftReg);
    freeReg(rightReg);
    freeReg(dstReg);
}

void MipsInstructionBuilder::genDivMod() {
    BinInstruction* bin = dynamic_cast<BinInstruction*>(this->irInstruction);
    std::string leftName = bin->getLeft();
    std::string rightName = bin->getRight();
    MipsSymbol* leftSb;
    MipsSymbol* rightSb;
    int leftReg = -1,rightReg = -1;
    /*左边的srcReg*/
    if (isGlobalSymbol(leftName)) {
        leftSb = this->mipsSymbolTable->findMipsSymbol(leftName);
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        La *la = new La(leftReg,leftName);
        Lw *lw = new Lw(leftReg,leftSb->getOffset(),leftSb->getBase());
        addIntoMipsTable(la);
        addIntoMipsTable(lw);
    }
    else if (isPartSymbol(leftName)) {
        leftSb = this->mipsSymbolTable->findMipsSymbol(leftName);
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        Lw *lw = new Lw(leftReg,leftSb->getOffset(),leftSb->getBase());
        addIntoMipsTable(lw);

    }
    else {
        leftReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(leftReg);
        Li *li = new Li(leftReg,std::stoi(leftName));
        this->mipsInstrList.emplace_back(li);

    }
    if (isGlobalSymbol(rightName)) {
        rightName.erase(0,1);
        rightSb = this->mipsSymbolTable->findMipsSymbol(rightName);
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        La *la = new La(rightReg,rightName);
        Lw *lw = new Lw(rightReg,rightSb->getOffset(),rightSb->getBase());
        addIntoMipsTable(la);
        addIntoMipsTable(lw);
    }
    else if (isPartSymbol(rightName)) {
        rightSb = this->mipsSymbolTable->findMipsSymbol(rightName);
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        Lw *lw = new Lw(rightReg,rightSb->getOffset(),rightSb->getBase());
        addIntoMipsTable(lw);

    }
    else {
        rightReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(rightReg);
        Li *li = new Li(rightReg,std::stoi(rightName));
        this->mipsInstrList.emplace_back(li);
    }
    int dstReg = -1;
    if (bin->getInstrType() == InstructionType::SDIV) {
        dstReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(dstReg);
        MipsSymbol* dstSb = new MipsSymbol(bin->getName(),29,this->mipsSymbolTable);
        giveMemory(dstSb,4);
        Div *div = new Div(dstReg,leftReg,rightReg);
        this->mipsInstrList.emplace_back(div);
        MipsSymbol *resultSb = new MipsSymbol(bin->getName(),29,this->mipsSymbolTable);
        giveMemory(resultSb,4);
        this->mipsSymbolTable->addMipsSymbol(resultSb);
        Sw *sw = new Sw(dstReg,resultSb->getOffset(),resultSb->getBase());
        this->mipsInstrList.emplace_back(sw);
        freeReg(dstReg);
        freeReg(leftReg);
        freeReg(rightReg);
    }
    else if (bin->getInstrType() == InstructionType::MOD) {
        dstReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(dstReg);
        Div *div = new Div(-1,leftReg,rightReg);
        MipsSymbol* dstSb = new MipsSymbol(bin->getName(),29,this->mipsSymbolTable);
        giveMemory(dstSb,4);
        Mfhi *mfhi = new Mfhi(dstReg);
        this->mipsInstrList.emplace_back(div);
        this->mipsInstrList.emplace_back(mfhi);
        MipsSymbol *resultSb = new MipsSymbol(bin->getName(),29,this->mipsSymbolTable);
        giveMemory(resultSb,4);
        this->mipsSymbolTable->addMipsSymbol(resultSb);
        Sw *sw = new Sw(dstReg,resultSb->getOffset(),resultSb->getBase());
        this->mipsInstrList.emplace_back(sw);
        freeReg(dstReg);
        freeReg(leftReg);
        freeReg(rightReg);
    }

}

void MipsInstructionBuilder::genCall() {
    Call *call = dynamic_cast<Call*>(this->irInstruction);
    if (call->getFuncType() == CALL_PUTINT) {
        std::string srcName = call->getPutName();
        if (isNumberSymbol(srcName)) {
            Li * li = new Li(strReg2intReg("$a0"),std::stoi(srcName));
            Li *putLi = new Li(strReg2intReg("$v0"),1);
            Syscall * syscall = new Syscall();
            this->mipsInstrList.emplace_back(li);
            this->mipsInstrList.emplace_back(putLi);
            this->mipsInstrList.emplace_back(syscall);
            freeReg(strReg2intReg("$v0"));

        }
        else if (isGlobalSymbol(srcName)){
            srcName.erase(0,1);
            La* la = new La(strReg2intReg("$a0"),srcName);
            Lw *lw = new Lw(strReg2intReg("$a0"),0,strReg2intReg("$a0"));
            Li *putLi = new Li(strReg2intReg("$v0"),1);
            Syscall * syscall = new Syscall();
            this->mipsInstrList.emplace_back(la);
            this->mipsInstrList.emplace_back(lw);
            this->mipsInstrList.emplace_back(putLi);
            this->mipsInstrList.emplace_back(syscall);
            freeReg(strReg2intReg("$v0"));
        }
        else if (isPartSymbol(srcName)) {
            MipsSymbol * srcSb = this->mipsSymbolTable->findMipsSymbol(srcName);
            Lw *lw = new Lw(strReg2intReg("$a0"),srcSb->getOffset(),srcSb->getBase());
            Li *putLi = new Li(strReg2intReg("$v0"),1);
            Syscall *syscall = new Syscall();
            this->mipsInstrList.emplace_back(lw);
            this->mipsInstrList.emplace_back(putLi);
            this->mipsInstrList.emplace_back(syscall);
            freeReg(strReg2intReg("$v0"));
        } else {std::cout << "CALL_PUTINT ERROR\n";}
    }
    else if (call->getFuncType() == CALL_PRIMARY) {
        std::cout << "start call\n";
        setRegUse(31);
        int paramNum = call->getParamNum();
        int need_reg_offset = 0;
        for (int i = 1;i <= 3;i++) {
            setRegUse(strReg2intReg("$a0") + i);
        }
        /*将在使用的寄存器压入栈中*/
        std::stack<int> regSt;
        for(int i = 1;i <= 31;i++) {
            if (this->mipsSymbolTable->isUsed(i)) {
                Sw *sw = new Sw(i,need_reg_offset + this->mipsSymbolTable->getSpTop(),strReg2intReg("$sp"));
                this->mipsInstrList.emplace_back(sw);
                need_reg_offset -= 4;
                regSt.push(i);
                freeReg(i);
            }
        }
        /*将a1,a2,a3三个寄存器存储参数*/
        for (int i = 1;i <= 3 && i <= paramNum;i++) {
            std::string paramRName = call->getParamRName(i);
            if (isNumberSymbol(paramRName)) {
                Li *li = new Li(strReg2intReg("$a0") + i,std::stoi(paramRName));
                this->mipsInstrList.emplace_back(li);
            }
            else if (isPartSymbol(paramRName)) {
                MipsSymbol *paramSb = this->mipsSymbolTable->findMipsSymbol(paramRName);
                Lw *lw = new Lw(strReg2intReg("$a0") + i,paramSb->getOffset(),paramSb->getBase());
                this->mipsInstrList.emplace_back(lw);
            }
            else if (isGlobalSymbol(paramRName)) {
                paramRName.erase(0,1);
                La *la = new La(strReg2intReg("$a0") + i,paramRName);
                Lw *lw = new Lw(strReg2intReg("$a0") + i,0,strReg2intReg("$a0") + i);
                this->mipsInstrList.emplace_back(la);
                this->mipsInstrList.emplace_back(lw);
            }else {std::cout << "CALL_PRIMARY ERROR\n";}
        }

        Addi *addi = new Addi(strReg2intReg("$sp"),strReg2intReg("$sp"),need_reg_offset + this->mipsSymbolTable->getSpTop());
        this->mipsInstrList.emplace_back(addi);


        /*将要使用的超过3的参数压入stack 的sp*/
        int new_sp_offset = 0;
        for (int i = 4;i <= paramNum;i++) {
            std::string paramRName = call->getParamRName(i);
            if (isNumberSymbol(paramRName)) {
                int tmpReg = this->mipsSymbolTable->findFreeReg();
                setRegUse(tmpReg);
                Li *li = new Li(tmpReg,std::stoi(paramRName));
                Sw *sw = new Sw(tmpReg,new_sp_offset,strReg2intReg("$sp"));
                new_sp_offset -= 4;
                this->mipsInstrList.emplace_back(li);
                this->mipsInstrList.emplace_back(sw);
                freeReg(tmpReg);
            }
            else if (isGlobalSymbol(paramRName)) {
                paramRName.erase(0,1);
                int tmpReg = this->mipsSymbolTable->findFreeReg();
                setRegUse(tmpReg);
                La *la = new La(tmpReg,paramRName);
                Sw *sw = new Sw(tmpReg,new_sp_offset,strReg2intReg("$sp"));
                this->mipsInstrList.emplace_back(la);
                this->mipsInstrList.emplace_back(sw);
                new_sp_offset -= 4;
                freeReg(tmpReg);
            }
            else if (isPartSymbol(paramRName)) {
                MipsSymbol *tmpSb = this->mipsSymbolTable->findMipsSymbol(paramRName);
                int tmpReg = this->mipsSymbolTable->findFreeReg();
                setRegUse(tmpReg);
                Lw *lw = new Lw(tmpReg,-(need_reg_offset + this->mipsSymbolTable->getSpTop())+tmpSb->getOffset(),tmpSb->getBase());
                Sw *sw = new Sw(tmpReg,new_sp_offset,strReg2intReg("$sp"));
                this->mipsInstrList.emplace_back(lw);
                this->mipsInstrList.emplace_back(sw);
                new_sp_offset -= 4;
                freeReg(tmpReg);
            } else {std::cout << "error in CALL_PRIMARY ERROR 2\n";}
        }

        /*jal*/
        std::string label_name = call->getFuncLabel();
        label_name.erase(0,1);//删除@
        Jal *jal = new Jal(label_name);
        this->mipsInstrList.emplace_back(jal);
        Addi *addi2 = new Addi(strReg2intReg("$sp"),strReg2intReg("$sp"),-(need_reg_offset + this->mipsSymbolTable->getSpTop()));
        this->mipsInstrList.emplace_back(addi2);

        /*将所有要用的栈压回去*/
        int sp_offset = need_reg_offset + this->mipsSymbolTable->getSpTop();
        while(!regSt.empty()) {
            Lw *lw = new Lw(regSt.top(),sp_offset + 4,strReg2intReg("$sp"));
            this->mipsInstrList.emplace_back(lw);
            setRegUse(regSt.top());
            regSt.pop();
            sp_offset += 4;
        }

        /*如果有返回值，将返回值的值存入v0*/
        if (!call->getName().empty()) {
            std::string retName = call->getName();
            MipsSymbol *retSb = new MipsSymbol(retName,29,this->mipsSymbolTable);
            giveMemory(retSb, 4);
            this->mipsSymbolTable->addMipsSymbol(retSb);
            int tmpReg = this->mipsSymbolTable->findFreeReg();
            setRegUse(tmpReg);
            Sw * sw = new Sw(strReg2intReg("$v0"),retSb->getOffset(),retSb->getBase());
            this->mipsInstrList.emplace_back(sw);
            this->freeReg(tmpReg);
            freeReg(strReg2intReg("$v0"));
        }

    }
    else if (call->getFuncType() == CALL_GETINT) {
        MipsSymbol *getIntSb = new MipsSymbol(call->getName(),29,this->mipsSymbolTable);
        giveMemory(getIntSb,4);
        this->mipsSymbolTable->addMipsSymbol(getIntSb);

        Li *li = new Li(strReg2intReg("$v0"),5);
        Syscall *syscall = new Syscall();
        Sw *sw = new Sw(strReg2intReg("$v0"),getIntSb->getOffset(),getIntSb->getBase());

        this->mipsInstrList.emplace_back(li);
        this->mipsInstrList.emplace_back(syscall);
        this->mipsInstrList.emplace_back(sw);
    }
}

void MipsInstructionBuilder::genRet() {
    Return *ret = dynamic_cast<Return*>(this->irInstruction);
    std::string retName = ret->getRet();
    if (retName == "void") {
        Jr *jr = new Jr();
        this->mipsInstrList.emplace_back(jr);
        return ;
    }
    if (this->mipsSymbolTable->ifMainFunc()) {
        Li * li = new Li(strReg2intReg("$v0"),10);
        Syscall *syscall = new Syscall();
        this->mipsInstrList.emplace_back(li);
        this->mipsInstrList.emplace_back(syscall);
        return;
    }
    if (isNumberSymbol(retName)) {
        Li *li = new Li(strReg2intReg("$v0"),std::stoi(retName));
        this->mipsInstrList.emplace_back(li);
    }
    else if (isGlobalSymbol(retName)) {
        retName.erase(0,1);
        La *la = new La(strReg2intReg("$v0"),retName);
        this->mipsInstrList.emplace_back(la);
    }
    else if (isPartSymbol(retName)) {
        MipsSymbol *retSb = this->mipsSymbolTable->findMipsSymbol(retName);
        Lw * lw = new Lw(strReg2intReg("$v0"),retSb->getOffset(),retSb->getBase());
        this->mipsInstrList.emplace_back(lw);
    } else {std::cout << "error in gnRet\n";}

    Jr *jr = new Jr();
    this->mipsInstrList.emplace_back(jr);
}



void MipsInstructionBuilder::genIcmp() {
    std::cout << "icmp start\n";
    Icmp *icmp = dynamic_cast<Icmp*>(this->irInstruction);
    MipsSymbol* cmpSb = new MipsSymbol(icmp->getName(),29,this->mipsSymbolTable);
    giveMemory(cmpSb,4);
    this->mipsSymbolTable->addMipsSymbol(cmpSb);
    int opType = icmp->getOpType();
    int leftReg = this->mipsSymbolTable->findFreeReg();
    setRegUse(leftReg);
    int rightReg = this->mipsSymbolTable->findFreeReg();
    setRegUse(rightReg);
    int dstReg = this->mipsSymbolTable->findFreeReg();
    setRegUse(dstReg);
    std::string leftName = icmp->getLeft();
    std::string rightName = icmp->getRight();
    std::cout << "start fetch\n";
    std::cout << "leftName == " << leftName << '\n';
    fetchValue(leftReg,leftName);

    std::cout << "middle fetch\n";
    fetchValue(rightReg,rightName);
    std::cout << "finish fetch\n";
    Cmp *cmp = new Cmp(opType,dstReg,leftReg,rightReg);
    std::cout << "cmp output == " << cmp->mipsOutput() << '\n';
    addIntoMipsTable(cmp);
    Sw *sw = new Sw(dstReg,cmpSb->getOffset(),cmpSb->getBase());
    addIntoMipsTable(sw);
    freeReg(leftReg);
    freeReg(rightReg);
    freeReg(dstReg);
    std::cout << "finish icmp\n";
}

void MipsInstructionBuilder::genBr() {
    Br *br = dynamic_cast<Br*>(this->irInstruction);

    if (br->getUserList().size() == 3) {//br %instr label %label1 ,%label2
        std::string condName = br->getUserList().at(0)->getValueName();
        int condReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(condReg);
        fetchValue(condReg,condName);
        std::string label1Name = br->getUserList().at(1)->getValueName();
        std::string label2Name = br->getUserList().at(2)->getValueName();
        label1Name.erase(0,1);
        label2Name.erase(0,1);
        std::string funcName = this->mipsSymbolTable->getFuncName();
        label1Name = funcName + "_" + label1Name;
        label2Name = funcName + "_" + label2Name;
        int zeroReg = this->mipsSymbolTable->findFreeReg();
        setRegUse(zeroReg);
        Li *li = new Li(zeroReg,0);
        Bne *bne = new Bne(condReg,zeroReg,label1Name);
        J* j = new J(label2Name);
        addIntoMipsTable(li);
        addIntoMipsTable(bne);
        addIntoMipsTable(j);
        freeReg(zeroReg);
        freeReg(condReg);
    }
    else if (br->getUserList().size() == 1) {
        std::string funcName = this->mipsSymbolTable->getFuncName();
        std::string labelName = br->getUserList().at(0)->getValueName();
        labelName.erase(0,1);
        labelName = funcName + "_" + labelName;
        J* j = new J(labelName);
        addIntoMipsTable(j);
    }
    else {
        std::cout << "genBr error\n";
    }
}

void MipsInstructionBuilder::genZext() {
    Zext *zext = dynamic_cast<Zext*>(this->irInstruction);
    std::string zextName = zext->getName();
    std::string primaryName = zext->getUserList().at(0)->getValueName();
    MipsSymbol *primarySb = this->mipsSymbolTable->findMipsSymbol(primaryName);
    primarySb->setMipsSymbolName(zextName);
    mipsSymbolTable->eraseSb(primaryName);
    mipsSymbolTable->addMipsSymbol(primarySb);
}




/**
 * 以下是一些用来辅助的函数
 * */

std::string MipsInstructionBuilder::irName() {
    return this->irInstruction->getName();
}

/*分配内存*/
void MipsInstructionBuilder::giveMemory(MipsSymbol *tarSb,int size) {
    int tarBase = tarSb->getBase();
    if (tarBase == 29) {
        int top = this->mipsSymbolTable->getSpTop();
        tarSb->setOffset(top);
        mipsSymbolTable->addSp(size);
    }
    else if (tarBase == 28){
        int top = this->mipsSymbolTable->getGpTop();
        tarSb->setOffset(top);
        mipsSymbolTable->addGp(size);
    }
    else {
        std::cout << "giveMemory Error\n";
    }
}


bool MipsInstructionBuilder::isNumberSymbol(std::string valName) {
    return valName[0] != '%' && valName[0]!= '@';
}

bool MipsInstructionBuilder::isGlobalSymbol(std::string name) {
    return name[0] == '@';
}

bool MipsInstructionBuilder::isPartSymbol(std::string name) {
    return name[0] == '%';
}

void MipsInstructionBuilder::addIntoMipsTable(MipsInstruction* mipsSymbol) {
    this->mipsInstrList.emplace_back(mipsSymbol);
}

void MipsInstructionBuilder::freeReg(int regIndex) {
    this->mipsSymbolTable->setFreeReg(regIndex);
}

void MipsInstructionBuilder::setRegUse(int regIndex) {
    this->mipsSymbolTable->setRegUse(regIndex);
}















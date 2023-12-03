//
// Created by me on 2023/11/4.
//

#include "InstructionBuilder.h"
#include "Instruction.h"
#include "Alloc.h"
#include "Store.h"
#include "Load.h"
#include "Call.h"
#include "Return.h"
#include "GEP.h"
#include "AllocInstruction.h"
#include "BinInstruction.h"
#include "../ValueType/ArrayType.h"
#include "../ValueType/NumberType.h"
InstructionBuilder::InstructionBuilder(Node *curNode, SymbolTable *curTable, int *cnt) {
    this->curNode = curNode;
    this->curTable = curTable;
    this->cnt = cnt;
}

void InstructionBuilder::genInstr(Node *tmpNode) {
    if (isStmt(*tmpNode)) {
        genStmt(tmpNode);
    }
    else if (isDecl(*tmpNode)) {
        genDecl(tmpNode);
    }
    else {
        //invalid
    }
}
void InstructionBuilder::genStmt(Node *tmpNode) {
    if (isLVal(tmpNode->getChildren()[0])) {
        genAssign(tmpNode);
    }
    else if (isPrintf(tmpNode->getChildren()[0])) {
        genPrintf(tmpNode);
    }
    else if (isBlock(tmpNode->getChildren()[0])) {//Block
        Node childNode = tmpNode->getChildren()[0];//block
        SymbolTable *newTable = new SymbolTable(this->curTable);
        SymbolTable* tmpTable = this->curTable;
        this->curTable = newTable;
        for (auto & tmp : childNode.getChildren()) {//tmp = blockItem
            if (isBlockItem(tmp)) {
                genInstr(&tmp.getChildren()[0]);//tmp.child = stmt | decl
            }
        }
        this->curTable = tmpTable;//回溯
    }
    else if (isReturn(tmpNode->getChildren()[0])) {
        genReturn(tmpNode);
    }
    else if(isExp(tmpNode->getChildren()[0])) {
        genExp(&tmpNode->getChildren()[0]);
    }
    else if (isIf(tmpNode->getChildren()[0])) {
        /*TODO*/
    }
    else if(isFor(tmpNode->getChildren()[0])) {
        /*TODO*/
    }
    else if(isBreak(tmpNode->getChildren()[0])) {
        /*TODO*/
    }
    else if(isContinue(tmpNode->getChildren()[0])) {
        /*TODO*/
    }
    else if (tmpNode->getChildren()[0].getLeafTokenName() == ";") {
        /*TODO*/
    }
    else {
        //Invalid
        std::cout << "invalid in genStmt in InstructionBuilder\n";
    }
}


void InstructionBuilder::genAssign(Node *tmpNode) {
    if (isExp(tmpNode->getChildren()[2])) {//LVal = Exp;
        IrValue *dst = genLVal(&tmpNode->getChildren()[0],true);//dst
        IrValue *val = genExp(&tmpNode->getChildren().at(2));//val
        Store *storeInstr = new Store(val,dst);
        this->instrList.emplace_back(storeInstr);
    }
    else if(isGetint(tmpNode->getChildren()[2])) {//LVal = getint();
        IrValue *dst = genLVal(&tmpNode->getChildren()[0],true);
        Call *callInstr = new Call(CALL_GETINT);
        callInstr->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back((Instruction*)callInstr);
        auto *storeInstr = new Store((IrValue*)callInstr,dst);
        this->instrList.emplace_back(storeInstr);
    }
}

/*LVal → Ident {'[' Exp ']'}*/
IrValue* InstructionBuilder::genLVal(Node *tmpNode,bool ifLeft) {//是否左值判断的是需不需要load指令
    int dim = tmpNode->calDim();
    std::string identName = tmpNode->getChildren()[0].getLeafTokenName();
    Symbol* tarSb = this->curTable->findSymbol(identName);
    IrValue* tarValue = tarSb->getIrValue();
    IrValue *finalValue = nullptr;
    if (dim == 0) {
        if (tarSb->getDim() == 1 || tarSb->getDim() == 2) {
            PointerType *pt = dynamic_cast<PointerType*>(tarValue->getValueType());
            std::vector<std::string> offsetList;
            if (pt->getContainedIrValueType() == IrValueType::ARRAY) {
                offsetList.emplace_back("0");
            }
            offsetList.emplace_back("0");
            GEP *gep = new GEP(pt->getContained(),tarValue,offsetList);
            gep->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            this->instrList.emplace_back(gep);
            return gep;
        }
        finalValue = tarValue;
    }
    else if (dim == 1) {
        /*获取地址偏移量*/
        IrValue *addressValue = genExp(&tmpNode->getChildren().at(2));
//        for (auto child : tmpNode->getChildren()) {
//            if (isExp(child)) {
//                addressValue = genExp(&child);
//            }
//        }
        //判断是i32*还是[3xi32]*
        std::vector<std::string> offsetList;
        PointerType *pt = dynamic_cast<PointerType*>(tarValue->getValueType());
        if (pt->getContainedIrValueType() == IrValueType::ARRAY) {
            offsetList.emplace_back("0");
        }
        if (tarSb->getDim() ==2) {
            IrValue *numberValue = new IrValue(std::to_string(tarSb->getDimList()[1]));
            BinInstruction *mul = new BinInstruction(addressValue,numberValue,InstructionType::MUL);
            mul->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            this->instrList.emplace_back(mul);
            offsetList.emplace_back(mul->getName());
            GEP *gep = new GEP(pt->getContained(),tarValue,offsetList);
            gep->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            this->instrList.emplace_back(gep);
            return gep;
        }
        offsetList.emplace_back(addressValue->getName());
        GEP *gep = new GEP(pt->getContained(),tarValue,offsetList);
        gep->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(gep);
        finalValue = gep;
    }
    else if (dim == 2) {
        std::vector<IrValue*> addressList;
        for (auto child : tmpNode->getChildren()) {
            if (isExp(child)) {
                addressList.emplace_back(genExp(&child));
            }
        }
        IrValue *dimValue = new IrValue(std::to_string(tarSb->getDimList()[1]));
        BinInstruction *mul = new BinInstruction(addressList[0],dimValue,InstructionType::MUL);
        mul->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(mul);
        BinInstruction *add = new BinInstruction(mul,addressList[1],InstructionType::ADD);
        add->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(add);
        //判断是i32*还是[3xi32]*
        std::vector<std::string> offsetList;
        PointerType *pt = dynamic_cast<PointerType*>(tarValue->getValueType());
        if (pt->getContainedIrValueType() == IrValueType::ARRAY){
            offsetList.emplace_back("0");
        }
        offsetList.emplace_back(add->getName());
        GEP *gep = new GEP(pt->getContained(),tarValue,offsetList);
        gep->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(gep);
        finalValue = gep;
    } else{std::cout<<"genLVal isLeft error\n";}
    if (ifLeft) {
        return finalValue;
    }
    Load *load = new Load(finalValue);
    load->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
    this->instrList.emplace_back(load);
    return load;
}

IrValue* InstructionBuilder::genExp(Node *tmpNode) {
    IrValue *value = genAddExp(&tmpNode->getChildren()[0]);
    return value;
}

IrValue *InstructionBuilder::genAddExp(Node *tmpNode) {
    if (isAddExp(tmpNode->getChildren()[0])) { //AddExp -> AddExp ('+' | '-') MulExp
        IrValue *addValue = genAddExp(&tmpNode->getChildren()[0]);
        IrValue *mulValue = genMulExp(&tmpNode->getChildren()[2]);
        std::string op = tmpNode->getChildren()[1].getLeafTokenName();
        BinInstruction* binInstr;
        if (op == "+") {
            binInstr = new BinInstruction(addValue,mulValue,InstructionType::ADD);
        }
        else {
            binInstr = new BinInstruction(addValue,mulValue,InstructionType::SUB);
        }
        int newCnt = getFuncCnt(this->cnt);
        binInstr->setName("%instr_" + std::to_string(newCnt));
        this->instrList.emplace_back(binInstr);
        return binInstr;
    }
    else {//AddExp -> MulExp
        IrValue *value = genMulExp(&tmpNode->getChildren()[0]);
        return value;
    }
    return nullptr;
}

IrValue *InstructionBuilder::genMulExp(Node *tmpNode) {
    if (isMulExp(tmpNode->getChildren()[0])) { //MulExp -> MulExp ('*' | '/' | '%') UnaryExp
        IrValue *mulValue = genMulExp(&tmpNode->getChildren()[0]);
        IrValue *unaryValue = genUnaryExp(&tmpNode->getChildren()[2]);
        std::string op = tmpNode->getChildren()[1].getLeafTokenName();
        BinInstruction* binInstr;
        if (op == "*") {
            binInstr = new BinInstruction(mulValue,unaryValue,InstructionType::MUL);
        }
        else if (op == "/"){
            binInstr = new BinInstruction(mulValue,unaryValue,InstructionType::SDIV);
        }
        else{
            binInstr = new BinInstruction(mulValue,unaryValue,InstructionType::MOD);

        }
        int newCnt = getFuncCnt(this->cnt);
        binInstr->setName("%instr_" + std::to_string(newCnt));
        this->instrList.emplace_back(binInstr);
        return binInstr;
    }
    else {//MulExp -> UnaryExp
        IrValue *value = genUnaryExp(&tmpNode->getChildren()[0]);
        return value;
    }
    return nullptr;
}

/*UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp*/
IrValue* InstructionBuilder::genUnaryExp(Node *tmpNode) {
    if (isPrimaryExp(tmpNode->getChildren()[0])) {//UnaryExp → PrimaryExp
        return genPrimaryExp(&tmpNode->getChildren()[0]);
    }
    else if (isUnaryOp(tmpNode->getChildren()[0])) {//UnaryOp UnaryExp
        if (tmpNode->getChildren()[0].getChildren()[0].getLeafTokenName() == "-") {
            auto *left = new IrValue("0");
            IrValue *right = genUnaryExp(&tmpNode->getChildren()[1]);
            auto *subInstr = new BinInstruction(left,right,InstructionType::SUB);
            int nowCnt = getFuncCnt(this->cnt);
            subInstr->setName("%instr_" + std::to_string(nowCnt));
            this->instrList.emplace_back(subInstr);
            return subInstr;
        }
        else if (tmpNode->getChildren()[0].getChildren()[0].getLeafTokenName() == "+"){
            IrValue *right = genUnaryExp(&tmpNode->getChildren()[1]);
            return right;
        }
        else {
            IrValue *right = genUnaryExp(&tmpNode->getChildren()[1]);
            right->setIfTrue();
            return right;
        }
    }
    else {//UnaryExp -> Ident '(' [FuncRParams] ')'
        std::vector<IrValue*> paramRList;
        if (isFuncRParams(tmpNode->getChildren().at(2))) {
            Node funcRParamsNode = tmpNode->getChildren().at(2);
            for (auto &tmpChild : funcRParamsNode.getChildren()) {
                if (isExp(tmpChild)) {
                    paramRList.emplace_back(genExp(&tmpChild));
                }
            }
        }
        std::string funcName = tmpNode->getChildren().at(0).getLeafTokenName();
        Symbol* funcSb = this->curTable->findSymbol(funcName);
        Function* func = dynamic_cast<Function*>(funcSb->getIrValue());
        Call *call = new Call((IrValue*)func,paramRList,CALL_PRIMARY);
        if (func->getReturnType() == "i32") {
            call->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            call->setValueType(new IntegerType(32));
        }
        else {
            call->setName("");
        }
        this->instrList.emplace_back(call);
        return call;
    }
    return nullptr;
}

/*PrimaryExp → '(' Exp ')' | LVal | Number*/
IrValue* InstructionBuilder::genPrimaryExp(Node *tmpNode) {
    if (isNumber(tmpNode->getChildren()[0])) {//PrimaryExp->Number
        return genNumber(&tmpNode->getChildren()[0]);
    }
    else if (isLVal(tmpNode->getChildren()[0])) {//PrimaryExp->LVal
        return genLVal(&tmpNode->getChildren()[0],false);
    }
    else {
        return genExp(&tmpNode->getChildren()[1]);
    }
    return nullptr;
}

IrValue* InstructionBuilder::genNumber(Node *tmpNode) {
    std::string name = tmpNode->getChildren()[0].getLeafTokenName();
    auto* numberValue = new IrValue(name);
    numberValue->setValueType(new NumberType());
    return numberValue;
}

/*'printf''('FormatString{','Exp}')'';'*/
void InstructionBuilder::genPrintf(Node *tmpNode) {
    std::vector<Node> expNodeList;
    for (auto& tmp : tmpNode->getChildren()) {
        if (isExp(tmp)) {
            expNodeList.emplace_back(tmp);
        }
    }
    std::vector<IrValue*> expResultList;
    expResultList.reserve(expNodeList.size());
for (auto tmp : expNodeList) {
        expResultList.emplace_back(genExp(&tmp));
    }
    std::string formatString = tmpNode->getChildren()[2].getLeafTokenName();
    int expNodeListPos = 0;
    for (int i = 1;i < formatString.size()-1;i++) {
        if (formatString[i] == '%') {
            IrValue* expValue = expResultList.at(expNodeListPos++);
            //IrValue *expValue = genExp(&expNodeList[expNodeListPos++]);
            Call *callInstr = new Call(expValue->getName(),CALL_PUTINT);
            callInstr->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            this->instrList.emplace_back((Instruction*)callInstr);
            i++;
        }
        else if (formatString[i] == '\\') {
            Call *callInstr = new Call('\n',CALL_PUTCH);
            i++;
            this->instrList.emplace_back((Instruction*)callInstr);
        }
        else {
            Call *callInstr = new Call(formatString[i],CALL_PUTCH);
            this->instrList.emplace_back((Instruction*)callInstr);
        }
    }
}

void InstructionBuilder::genDecl(Node *tmpNode) {
    if (isConstDecl(tmpNode->getChildren()[0])) {
        genConstDecl(&tmpNode->getChildren()[0]);
    }
    else {
        genVarDecl(&tmpNode->getChildren()[0]);
    }
}

void InstructionBuilder::genConstDecl(Node *tmpNode) {
    for (auto &child : tmpNode->getChildren()) {
        if (isConstDef(child)) {
            genConstDef(&child);
        }
    }
}

/*ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal*/
void InstructionBuilder::genConstDef(Node *tmpNode) {
    std::string identName = tmpNode->getChildren()[0].getLeafTokenName();
    int dim = tmpNode->calDim();
    std::vector<int> constInitVal = tmpNode->getChildren().back().calConstInitVal(this->curTable);
    SymbolCon *constSymbol;
    std::vector<int> dimList;
    for (auto child : tmpNode->getChildren()) {
        if (isConstExp(child)) {
            dimList.emplace_back(child.calConstExp(this->curTable));
        }
    }
    if (dim == 0) {
        auto *alloc = new Alloc();
        alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(alloc);
        constSymbol = new SymbolCon(identName,constInitVal[0],alloc,this->curTable);
        this->curTable->addSymbol((Symbol*)constSymbol);
        auto *numValue = new IrValue(std::to_string(constInitVal[0]));
        numValue->setValueType(new NumberType());
        auto *store = new Store(numValue,alloc);
        this->instrList.emplace_back(store);
    }
    else if (dim == 1) {
        /*ALLOC*/
        Alloc *alloc = new Alloc(constInitVal.size());
        alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(alloc);
        SymbolCon *irSymbol = new SymbolCon(identName,constInitVal,alloc,this->curTable);
        irSymbol->setDimList(dimList[0]);
        this->curTable->addSymbol(irSymbol);
        /*getelementptr AND store*/
        for (int i = 0;i < constInitVal.size();i++) {
            std::vector<std::string> offList = {"0",std::to_string(i)};
            GEP *gep = new GEP(alloc->getPtContainedType(),alloc,offList);
            gep->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            this->instrList.emplace_back(gep);
            IrValue *value = new IrValue(std::to_string(constInitVal[i]));
            value->setValueType(new NumberType());
            Store *store = new Store(value,gep);
            this->instrList.emplace_back(store);
        }
    }
    else if (dim == 2) {
        Alloc *alloc = new Alloc(constInitVal.size());
        alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(alloc);
        std::vector<std::vector<int>> doubleConstInitVal = {constInitVal};
        SymbolCon *irSymbol = new SymbolCon(identName,doubleConstInitVal,alloc,this->curTable);
        irSymbol->setDimList(dimList);
        this->curTable->addSymbol(irSymbol);
        for(int i = 0;i < dimList[0];i++) {
            for (int j = 0;j < dimList[1];j++) {
                std::vector<std::string> offList = {"0",std::to_string(i * dimList[1] + j)};
                GEP *gep = new GEP(alloc->getPtContainedType(),alloc,offList);
                gep->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
                this->instrList.emplace_back(gep);
                IrValue *value = new IrValue(std::to_string(constInitVal[i * dimList[1] + j]));
                value->setValueType(new NumberType());
                Store *store = new Store(value,gep);
                this->instrList.emplace_back(store);
            }
        }
    } else {std::cout << "genConstDef error\n";}
}

void InstructionBuilder::genVarDecl(Node *tmpNode) {
    for (auto &child : tmpNode->getChildren()) {
        if (isVarDef(child)) {
            genVarDef(&child);
        }
    }
}

/*VarDef → Ident { '[' ConstExp ']' } | Ident { '[' ConstExp ']' } '=' InitVal*/
void InstructionBuilder::genVarDef(Node *tmpNode) {
    std::string name = tmpNode->getChildren()[0].getLeafTokenName();
    int dim = tmpNode->calDim();
    std::vector<int> dimList;
    for (auto child : tmpNode->getChildren()) {
        if (isConstExp(child)) {
            dimList.emplace_back(child.calConstExp(this->curTable));
        }
    }
    if (dim == 0) {
        Alloc *alloc = new Alloc();
        alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(alloc);
        auto *varSymbol = new SymbolVar(name,(IrValue*)alloc,this->curTable);
        this->curTable->addSymbol((Symbol*)varSymbol);
        if (isInitVal(tmpNode->getChildren().back())) {
            genInitVal(&tmpNode->getChildren().back(),(IrValue*)alloc);
        }

    }
    else if (dim == 1) {
        /*构造alloc*/
        Alloc *alloc = new Alloc(dimList[0]);
        alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(alloc);
        SymbolVar *varSb = new SymbolVar(name,alloc,this->curTable,dimList[0]);
        this->curTable->addSymbol(varSb);
        if (isInitVal(tmpNode->getChildren().back())) {
            genInitVal(&tmpNode->getChildren().back(),(IrValue*)alloc);
        }
    }
    else if (dim == 2) {
        Alloc *alloc = new Alloc(dimList[0] * dimList[1]);
        alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(alloc);
        SymbolVar *varSb = new SymbolVar(name,alloc,this->curTable,dimList[0],dimList[1]);
        this->curTable->addSymbol(varSb);
        if (isInitVal(tmpNode->getChildren().back())) {
            genInitVal(&tmpNode->getChildren().back(),(IrValue*)alloc);
        }
    }
}

/*InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'*/
void InstructionBuilder::genInitVal(Node *tmpNode,IrValue* tarValue) {
    if (isExp(tmpNode->getChildren()[0])) {
        IrValue *tmpVale = genExp(&tmpNode->getChildren()[0]);
        auto* tmpStore = new Store(tmpVale,tarValue);
        this->instrList.emplace_back(tmpStore);
    }
    else {
        int index = 0;
        for (auto child : tmpNode->getChildren()) {
            if (isInitVal(child)) {
                genArrayInitVal(index,&child,tarValue);
                index++;
            }
        }
    }
}

/*InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'*/
void InstructionBuilder::genArrayInitVal(int index,Node *tmpNode,IrValue *tarValue) {
    if (isExp(tmpNode->getChildren().at(0))) {
        auto ExpChild = tmpNode->getChildren().at(0);
        IrValue *tmpValue = genExp(&ExpChild);
        std::vector<std::string> indexList = {"0",std::to_string(index)};
        Alloc *alloc = dynamic_cast<Alloc*>(tarValue);
        GEP * gep = new GEP(alloc->getPtContainedType(),tarValue,indexList);
        gep->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
        this->instrList.emplace_back(gep);
        Store *store = new Store(tmpValue,gep);
        this->instrList.emplace_back(store);
    }
    else {//说明是二维数组
        int indexNum = 0;
        for (auto child : tmpNode->getChildren()) {//第一遍遍历，计算initVal的个数，用来为第二遍确定好乘法的法则
            if (isInitVal(child)) {
                indexNum++;
            }
        }
        int subIndex = 0;
        for (auto child : tmpNode->getChildren()) {
            if (isInitVal(child)) {
                genArrayInitVal(index * indexNum + subIndex,&child,tarValue);
                subIndex++;
            }
        }
    }
}

void InstructionBuilder::genReturn(Node *tmpNode) {
    if (tmpNode->getChildren().size() == 2) {
        Return *ret = new Return("void");
        this->instrList.emplace_back(ret);
    }
    else {
        IrValue *value = genExp(&tmpNode->getChildren().at(1));
        Return *ret = new Return(value);
        this->instrList.emplace_back(ret);
    }
}

void InstructionBuilder::genFuncBlock() {//curNode -> block
    for (auto &child : this->curNode->getChildren()) {//child->blockItem
        genInstr(&(child.getChildren()[0]));//child.children -> decl | stmt
    }
}

std::vector<Instruction *> InstructionBuilder::getInstrList() {
    return this->instrList;
}

void InstructionBuilder::genSetZero(IrValue *tarValue) {
    IrValue *zeroValue = new IrValue("0");
    Store *store = new Store(zeroValue,tarValue);
    this->instrList.emplace_back(store);
}

void InstructionBuilder::genParamInstr(std::string identName) {
    for (auto sb : this->curTable->getSymbolList()) {
        Symbol* parmSb = sb.second;
        if (parmSb->getSymbolType() == VARIABLE_TYPE) {
            Alloc *alloc = new Alloc();
            alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
            IrValue *paramValue = parmSb->getIrValue();
            parmSb->setIrValue(alloc);
            this->instrList.emplace_back(alloc);
            Store *store = new Store(paramValue,alloc);
            this->instrList.emplace_back(store);
        }
        else if (parmSb->getSymbolType() == ARRAY_1_TYPE || parmSb->getSymbolType() == ARRAY_2_TYPE) {
            /*do nothing*/
//            IntegerType *integer1 = new IntegerType(32);
//            PointerType *subPt = new PointerType(integer1);
//            PointerType *pt = new PointerType(subPt);
//            Alloc *alloc = new Alloc(pt);
//            alloc->setName("%instr_" + std::to_string(getFuncCnt(this->cnt)));
//            IrValue* paramValue = parmSb->getIrValue();
//            parmSb->setIrValue(alloc);
//            this->instrList.emplace_back(alloc);
//            Store *store = new Store(paramValue,alloc);
//            this->instrList.emplace_back(store);
        } else {std::cout << "error in genParamInstr\n";}
    }

}












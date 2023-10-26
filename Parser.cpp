//
// Created by me on 2023/9/26.
//
#include "Parser.h"
Parser::Parser(const char *path) :lexer{path}, root{Node(nullptr,GrammarType::CompUnit,0)}{
    this->cursor = 0;
    this->fout.open("output.txt");
}

void Parser::output() {
    std::ofstream fileout("output.txt");
    std::vector<std::string> ans;
    this->root.fileWrite(ans);
    for (std::string tmp : ans) {
        fileout << tmp << '\n';
    }
}

/*Exp → AddExp*/
int Parser::parseExp(int pos, Node* tmpNode) {
    Node expNode = Node(tmpNode,GrammarType::Exp,pos);
    pos = parseAddExp(pos,&expNode);
    tmpNode->addChild(expNode);
    return pos;
}

/*Number → IntConst*/
int Parser::parseNumber(int pos, Node* tmpNode) {
    Node numberNode = Node(tmpNode,GrammarType::Number,pos);
    numberNode.addLeaf(tokens[pos],pos);pos++;
    tmpNode->addChild(numberNode);
    return pos;
} //ok

/*LVal → Ident {'[' Exp ']'}*/
int Parser::parseLVal(int pos, Node* tmpNode) {
    Node LValNode = Node(tmpNode,GrammarType::LVal,pos);
    LValNode.addLeaf(tokens[pos],pos);pos++;
    while(tokens[pos].getTokenName() == "[") {
        LValNode.addLeaf(tokens[pos],pos);pos++;
        pos = parseExp(pos,&LValNode);
        if (tokens[pos].getTokenName() == "]") {
            LValNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    tmpNode->addChild(LValNode);
    return pos;
}

/*PrimaryExp → '(' Exp ')' | LVal | Number*/
int Parser::parsePrimaryExp(int pos, Node* tmpNode) {
    Node primaryExpNode = Node(tmpNode,GrammarType::PrimaryExp,pos);
    if (tokens[pos].getTokenName() == "(") {
        primaryExpNode.addLeaf(tokens[pos],pos);pos++;
        pos = parseExp(pos,&primaryExpNode);
        primaryExpNode.addLeaf(tokens[pos],pos);pos++;
    }
    else if(tokens[pos].type2Str() == "INTCON") {
        pos = parseNumber(pos,&primaryExpNode);
    }
    else {
        pos = parseLVal(pos,&primaryExpNode);
    }
    tmpNode->addChild(primaryExpNode);
    return pos;
}//ok

/*FuncRParams → Exp { ',' Exp }*/
int Parser::parseFuncRParams(int pos, Node* tmpNode) {
    Node funcRParamsNode = Node(tmpNode,GrammarType::FuncRParams,pos);
    pos = parseExp(pos,&funcRParamsNode);
    while(tokens[pos].getTokenName() == ",") {
        funcRParamsNode.addLeaf(tokens[pos],pos);pos++;
        pos = parseExp(pos,&funcRParamsNode);
    }
    tmpNode->addChild(funcRParamsNode);
    return pos;
}

/*UnaryOp → '+' | '−' | '!'*/
int Parser::parseUnaryOp(int pos, Node *tmpNode) {
    Node unaryOpNode = Node(tmpNode,GrammarType::UnaryOp,pos);
    unaryOpNode.addLeaf(tokens[pos],pos);pos++;
    tmpNode->addChild(unaryOpNode);
    return pos;
}

/*UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp*/
int Parser::parseUnaryExp(int pos, Node* tmpNode) {
    Node unaryExpNode = Node(tmpNode,GrammarType::UnaryExp,pos);
    if (pos + 1 < tokens.size() && tokens[pos].getType() == TokenType::IDENFR && tokens[pos + 1].getTokenName() == "(") {
        unaryExpNode.addLeaf(tokens[pos],pos);pos++;
        unaryExpNode.addLeaf(tokens[pos],pos);pos++;
        if (tokens[pos].getType() == TokenType::PLUS ||
            tokens[pos].getType() == TokenType::MINU ||
            tokens[pos].getType() == TokenType::NOT ||
            tokens[pos].getType() == TokenType::IDENFR ||
            tokens[pos].getType() == TokenType::LPARENT ||
            tokens[pos].getType() == TokenType::INTCON) {
            pos = parseFuncRParams(pos,&unaryExpNode);
        }
        if (tokens[pos].getTokenName() == ")") {
            unaryExpNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    else if (tokens[pos].getTokenName() == "+" || tokens[pos].getTokenName() == "-" || tokens[pos].getTokenName() == "!"){
        pos = parseUnaryOp(pos,&unaryExpNode);
        pos = parseUnaryExp(pos,&unaryExpNode);
    }
    else {
        pos = parsePrimaryExp(pos,&unaryExpNode);
    }
    tmpNode->addChild(unaryExpNode);
    return pos;
}//ok

/*MulExp → UnaryExp | MulExp ('*' | '/' | '%')*/
int Parser::parseMulExp(int pos, Node* tmpNode) {
    std::stack<Node> st;
    Node baseMulExp = Node(nullptr,GrammarType::MulExp,pos);
    pos = parseUnaryExp(pos,&baseMulExp);
    st.push(baseMulExp);
    while(tokens[pos].getTokenName() == "*" || tokens[pos].getTokenName() == "/" || tokens[pos].getTokenName() == "%") {
        Node mulExpNode = Node(nullptr, GrammarType::MulExp, pos);
        mulExpNode.addChild(st.top());
        st.top().setParent(&mulExpNode);
        mulExpNode.addLeaf(tokens[pos], pos);pos++;
        pos = parseUnaryExp(pos, &mulExpNode);
        st.push(mulExpNode);
    }
    Node ansNode = st.top();
    st.pop();
    ansNode.setParent(tmpNode);
    tmpNode->addChild(ansNode);
    return pos;
}//ok

/*AddExp → MulExp | AddExp ('+' | '−') MulExp*/
int Parser::parseAddExp(int pos, Node* tmpNode) {
    std::stack<Node> st;
    Node baseAddExp = Node(nullptr,GrammarType::AddExp,pos);
    pos = parseMulExp(pos,&baseAddExp);
    st.push(baseAddExp);
    while(tokens[pos].getTokenName() == "+" || tokens[pos].getTokenName() == "-") {
        Node addExpNode = Node(nullptr, GrammarType::AddExp, pos);
        addExpNode.addChild(st.top());
        st.top().setParent(&addExpNode);
        addExpNode.addLeaf(tokens[pos], pos);pos++;

        pos = parseMulExp(pos, &addExpNode);
        st.push(addExpNode);
    }
    Node ansNode = st.top();
    st.pop();
    ansNode.setParent(tmpNode);
    tmpNode->addChild(ansNode);
    return pos;
}//ok

/*RelExp → AddExp | RelExp ('<' | '>' | '<=' | '>=') AddExp*/
int Parser::parseRelExp(int pos, Node *tmpNode) {
    std::stack<Node> st;
    Node baseRelExp = Node(nullptr,GrammarType::RelExp,pos);
    pos = parseAddExp(pos,&baseRelExp);
    st.push(baseRelExp);
    while(tokens[pos].getTokenName() == "<" || tokens[pos].getTokenName() == ">" ||
        tokens[pos].getTokenName() == "<=" || tokens[pos].getTokenName() == ">=") {
        Node relExpNode = Node(nullptr, GrammarType::RelExp, pos);
        relExpNode.addChild(st.top());
        st.top().setParent(&relExpNode);
        relExpNode.addLeaf(tokens[pos], pos);
        pos++;
        pos = parseAddExp(pos, &relExpNode);
        st.push(relExpNode);
    }
    Node ansNode = st.top();
    st.pop();
    ansNode.setParent(tmpNode);
    tmpNode->addChild(ansNode);
    return pos;
}

/*EqExp → RelExp | EqExp ('==' | '!=') RelExp*/
int Parser::parseEqExp(int pos, Node*tmpNode) {
    std::stack<Node> st;
    Node baseEqExp = Node(nullptr,GrammarType::EqExp,pos);
    pos = parseRelExp(pos,&baseEqExp);
    st.push(baseEqExp);
    while(tokens[pos].getTokenName() == "==" || tokens[pos].getTokenName() == "!=") {
        Node EqExp = Node(nullptr, GrammarType::EqExp, pos);
        EqExp.addChild(st.top());
        st.top().setParent(&EqExp);
        EqExp.addLeaf(tokens[pos], pos);
        pos++;
        pos = parseRelExp(pos, &EqExp);
        st.push(EqExp);
    }
    Node ansNode = st.top();
    st.pop();
    ansNode.setParent(tmpNode);
    tmpNode->addChild(ansNode);
    return pos;
}

/* LAndExp → EqExp | LAndExp '&&' EqExp*/
int Parser::parseLAndExp(int pos, Node *tmpNode) {
    std::stack<Node> st;
    Node baseLAndExp = Node(nullptr,GrammarType::LAndExp,pos);
    pos = parseEqExp(pos,&baseLAndExp);
    st.push(baseLAndExp);
    while(tokens[pos].getTokenName() == "&&") {
        Node LAndExp = Node(nullptr, GrammarType::LAndExp, pos);
        LAndExp.addChild(st.top());
        st.top().setParent(&LAndExp);
        LAndExp.addLeaf(tokens[pos], pos);
        pos++;
        pos = parseEqExp(pos, &LAndExp);
        st.push(LAndExp);
    }
    Node ansNode = st.top();
    st.pop();
    ansNode.setParent(tmpNode);
    tmpNode->addChild(ansNode);
    return pos;
}

/*LOrExp → LAndExp | LOrExp '||' LAndExp*/
int Parser::parseLOrExp(int pos, Node *tmpNode) {
    std::stack<Node> st;
    Node baseLOrExp = Node(nullptr,GrammarType::LOrExp,pos);
    pos = parseLAndExp(pos,&baseLOrExp);
    st.push(baseLOrExp);
    while(tokens[pos].getTokenName() == "||") {
        Node lOrExp = Node(nullptr, GrammarType::LOrExp, pos);
        lOrExp.addChild(st.top());
        st.top().setParent(&lOrExp);
        lOrExp.addLeaf(tokens[pos], pos);
        pos++;
        pos = parseLAndExp(pos, &lOrExp);
        st.push(lOrExp);
    }
    Node ansNode = st.top();
    st.pop();
    ansNode.setParent(tmpNode);
    tmpNode->addChild(ansNode);
    return pos;
}

/*ConstExp → AddExp*/
int Parser::parseConstExp(int pos, Node* tmpNode) {
    Node constExpNode = Node(tmpNode,GrammarType::ConstExp,pos);
    pos = parseAddExp(pos,&constExpNode);
    tmpNode->addChild(constExpNode);
    return pos;
}//ok

/*ConstInitVal → ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'*/
int Parser::parseConstInitVal(int pos, Node* tmpNode) {
    Node constInitValNode = Node(tmpNode,GrammarType::ConstInitVal,pos);
    if (tokens[pos].getTokenName() == "{") {
        constInitValNode.addLeaf(tokens[pos],pos);
        pos++;
        while(true) {
            if (tokens[pos].getTokenName() == "}") {
                constInitValNode.addLeaf(tokens[pos],pos);
                pos++;
                break;
            }
            else if (tokens[pos].getTokenName() == ",") {
                constInitValNode.addLeaf(tokens[pos],pos);
                pos++;
            }
            else {
                pos = parseConstInitVal(pos,&constInitValNode);
            }
        }
    }
    else {
        pos = parseConstExp(pos,&constInitValNode);
    }
    tmpNode->addChild(constInitValNode);
    return pos;
} //ok

/*ConstDef → Ident { '[' ConstExp ']' } '=' ConstInitVal*/
int Parser::parseConstDef(int pos, Node* tmpNode) {
    Node constDefNode = Node(tmpNode,GrammarType::ConstDef,pos);
    constDefNode.addLeaf(tokens[pos],pos);pos++;// Ident
    if (tokens[pos].getTokenName() == "[") {
        constDefNode.addLeaf(tokens[pos],pos);pos++;//[
        pos = parseConstExp(pos,&constDefNode);//constExp
        if (tokens[pos].getTokenName() == "]") {
            constDefNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    if (pos < tokens.size() && tokens[pos].getTokenName() == "[") {
        constDefNode.addLeaf(tokens[pos],pos);pos++;//[
        pos = parseConstExp(pos,&constDefNode);
        if (tokens[pos].getTokenName() == "]") {
            constDefNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    constDefNode.addLeaf(tokens[pos],pos);pos++;//=
    pos = parseConstInitVal(pos,&constDefNode);
    tmpNode->addChild(constDefNode);
    return pos;
} //ok

/*VarDef → Ident { '[' ConstExp ']' } | Ident { '[' ConstExp ']' } '=' InitVal */
int Parser::parseVarDef(int pos, Node* tmpNode) {
    Node varDefNode = Node(tmpNode,GrammarType::VarDef,pos);
    varDefNode.addLeaf(tokens[pos],pos);pos++;//Ident
    while (tokens[pos].getTokenName() == "[") {
        varDefNode.addLeaf(tokens[pos],pos);pos++;//'['
        pos = parseConstExp(pos,&varDefNode);
        if (tokens[pos].getTokenName() == "]") {
            varDefNode.addLeaf(tokens[pos],pos);pos++;//']'
        }
    }
    if (tokens[pos].getTokenName() == "=") {
        varDefNode.addLeaf(tokens[pos],pos);pos++;//=
        pos = parseInitVal(pos,&varDefNode);
    }
    tmpNode->addChild(varDefNode);
    return pos;
}

/*InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'*/
int Parser::parseInitVal(int pos, Node* tmpNode){
    Node initValNode = Node(tmpNode,GrammarType::InitVal,pos);
    if (tokens[pos].getTokenName() == "{") {
        initValNode.addLeaf(tokens[pos],pos);
        pos++;
        while(true) {
            if (tokens[pos].getTokenName() == "}") {
                initValNode.addLeaf(tokens[pos],pos);
                pos++;
                break;
            }
            else if (tokens[pos].getTokenName() == ",") {
                initValNode.addLeaf(tokens[pos],pos);
                pos++;
            }
            else {
                pos = parseInitVal(pos,&initValNode);
            }
        }
    }
    else {
        pos = parseExp(pos,&initValNode);
    }
    tmpNode->addChild(initValNode);
    return pos;
}

int Parser::parseDecl(int pos, Node* tmpNode) {
    Node declNode = Node(tmpNode,GrammarType::Decl,pos);
    if (tokens[pos].getTokenName() == "const") {//const declare
        pos = parseConstDecl(pos, &declNode);
    }
    else {//var declare
        pos = parseVarDecl(pos, &declNode);
    }
    tmpNode->addChild(declNode);
    return pos;
} //ok //re

/*ConstDecl → 'const' BType ConstDef { ',' ConstDef } ';'*/
int Parser::parseConstDecl(int pos, Node* tmpNode) {
    Node constDeclNode = Node(tmpNode,GrammarType::ConstDecl,pos);
    constDeclNode.addLeaf(tokens[pos],pos);pos++;//加入const这个终结符
    pos = parseBType(pos,&constDeclNode);//parse BType
    while(true) {
        pos = parseConstDef(pos,&constDeclNode);
        if (tokens[pos].getTokenName() == ",") {
            constDeclNode.addLeaf(tokens[pos],pos);pos++;
        }
        else if (tokens[pos].getTokenName() == ";") {
            constDeclNode.addLeaf(tokens[pos],pos);pos++;
            break;
        }
        else {
            break; //error
        }
    }
    tmpNode->addChild(constDeclNode);
    return pos;
}//ok

/*VarDecl → BType VarDef { ',' VarDef } ';'*/
int Parser::parseVarDecl(int pos, Node* tmpNode) {
    //writeFile(tokens[pos]);//int
    Node varDeclNode = Node(tmpNode,GrammarType::VarDecl,pos);
    pos = parseBType(pos,&varDeclNode);//BType
    pos = parseVarDef(pos,&varDeclNode);//VarDef
    while(true) {
        if (tokens[pos].getTokenName() == ",") {
            varDeclNode.addLeaf(tokens[pos],pos);pos++;
            pos = parseVarDef(pos,&varDeclNode);
        }
        else if (tokens[pos].getTokenName() == ";"){// ;
            varDeclNode.addLeaf(tokens[pos],pos);pos++;
            break;
        }
        else { //error
            break;
        }
    }
    tmpNode->addChild(varDeclNode);
    return pos;
} //ok

/*FuncDef → FuncType Ident '(' [FuncFParams] ')' Block*/
int Parser::parseFuncDef(int pos, Node* tmpNode) {
    Node funcDefNode(tmpNode,GrammarType::FuncDef,pos);//FuncType
    pos = parseFuncType(pos,&funcDefNode);
    funcDefNode.addLeaf(tokens[pos],pos);pos++;
    funcDefNode.addLeaf(tokens[pos],pos);pos++;
    if (tokens[pos].getTokenType() == TokenType::INTTK) {
        pos = parseFuncFParams(pos,&funcDefNode);//FuncFParams
    }
    if (tokens[pos].getTokenName() == ")") {
        funcDefNode.addLeaf(tokens[pos],pos);pos++;//)
    }
    pos = parseBlock(pos,&funcDefNode);
    tmpNode->addChild(funcDefNode);
    return pos;
}

/*FuncType → 'void' | 'int'*/
int Parser::parseFuncType(int pos, Node* tmpNode) {
    Node funcTypeNode = Node(tmpNode,GrammarType::FuncType,pos);
    funcTypeNode.addLeaf(tokens[pos],pos);
    tmpNode->addChild(funcTypeNode);
    pos++;
    return pos;
}

/*FuncFParams → FuncFParam { ',' FuncFParam }*/
int Parser::parseFuncFParams(int pos, Node *tmpNode) {
    Node funcFParamsNode = Node(tmpNode,GrammarType::FuncFParams,pos);
    pos = parseFuncFParam(pos,&funcFParamsNode);
    while(tokens[pos].getTokenName() == ",") {
        funcFParamsNode.addLeaf(tokens[pos],pos);
        pos++;
        pos = parseFuncFParam(pos, &funcFParamsNode);
    }
    tmpNode->addChild(funcFParamsNode);
    return pos;
}

/* FuncFParam → BType Ident ['[' ']' { '[' ConstExp ']' }] */
int Parser::parseFuncFParam(int pos,Node *tmpNode) {
    Node funcFParamNode = Node(tmpNode,GrammarType::FuncFParam,pos);
    pos = parseBType(pos,&funcFParamNode);//BType
    funcFParamNode.addLeaf(tokens[pos],pos);pos++; //Ident
    if (tokens[pos].getTokenName() == "[") {
        funcFParamNode.addLeaf(tokens[pos],pos);pos++;// [
        if (tokens[pos].getTokenName() == "]") {
            funcFParamNode.addLeaf(tokens[pos],pos);pos++;
        }
        while(tokens[pos].getTokenName() == "[") {
            funcFParamNode.addLeaf(tokens[pos],pos);pos++;//[
            pos = parseConstExp(pos,&funcFParamNode);
            if (tokens[pos].getTokenName() == "]") {
                funcFParamNode.addLeaf(tokens[pos],pos);pos++;
            }
        }
    }
    tmpNode->addChild(funcFParamNode);
    return pos;
}

/* MainFuncDef → 'int' 'main' '(' ')' Block*/
int Parser::parseMainFuncDef(int pos, Node* tmpNode) {
    Node mainFuncDefNode(tmpNode,GrammarType::MainFuncDef,pos);
    for (int i = pos;i <= pos + 3;i++) {
        mainFuncDefNode.addLeaf(tokens[i],pos);
    }
    pos = pos + 4;
    pos = parseBlock(pos,&mainFuncDefNode);
    tmpNode->addChild(mainFuncDefNode);
    return pos;
}

/*Block → '{' { BlockItem } '}'*/
int Parser::parseBlock(int pos, Node* tmpNode) {
    Node blockNode = Node(tmpNode,GrammarType::Block,pos);
    blockNode.addLeaf(tokens[pos],pos);pos++;
    while(tokens[pos].getTokenName() != "}") {
        pos = parseBlockItem(pos,&blockNode);
    }
    blockNode.addLeaf(tokens[pos],pos);pos++;
    tmpNode->addChild(blockNode);
    return pos;
}

/*BlockItem → Decl | Stmt*/
int Parser::parseBlockItem(int pos, Node *tmpNode) {
    Node blockItemNode = Node(tmpNode,GrammarType::BlockItem,pos);
    if (tokens[pos].getTokenName() == "int" || tokens[pos].getTokenName() == "const") {
        pos = parseDecl(pos,&blockItemNode);
    }
    else {
        pos = parseStmt(pos,&blockItemNode);
    }
    tmpNode->addChild(blockItemNode);
    return pos;
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
| 'printf''('FormatString{','Exp}')'';'
 */
int Parser::parseStmt(int pos, Node *tmpNode) {
    Node stmtNode = Node(tmpNode,GrammarType::Stmt,pos);
    if(tokens[pos].getTokenName() == "if" || tokens[pos].getTokenName() == "while") {//if
        stmtNode.addLeaf(tokens[pos],pos);pos++; //if
        stmtNode.addLeaf(tokens[pos],pos);pos++; //'('

        pos = parseCond(pos,&stmtNode);
        if (tokens[pos].getTokenName() == ")") {
            stmtNode.addLeaf(tokens[pos],pos);pos++;//)
        }

        pos = parseStmt(pos,&stmtNode);
        if (tokens[pos].getTokenName() == "else") {
            stmtNode.addLeaf(tokens[pos],pos);
            pos++;
            pos = parseStmt(pos,&stmtNode);
        }
    }
    else if(tokens[pos].getTokenName() == "for") {//for
        stmtNode.addLeaf(tokens[pos],pos);pos++;
        stmtNode.addLeaf(tokens[pos],pos);pos++;
        if (tokens[pos].getTokenName() != ";") {
            pos = parseForStmt(pos,&stmtNode);
        }
        stmtNode.addLeaf(tokens[pos],pos);pos++;//;
        if (tokens[pos].getTokenName() != ";") {
            pos = parseCond(pos,&stmtNode);
        }
        stmtNode.addLeaf(tokens[pos],pos);pos++;//;
        if (tokens[pos].getTokenName() != ")") {
            pos = parseForStmt(pos,&stmtNode);
        }
        stmtNode.addLeaf(tokens[pos],pos);pos++;//)
        pos = parseStmt(pos,&stmtNode);
    }
    else if (tokens[pos].getTokenName() == "break" || tokens[pos].getTokenName() == "continue") {//continue or break
        stmtNode.addLeaf(tokens[pos],pos);pos++;
        if (tokens[pos].getTokenName() == ";"){
            stmtNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    else if (tokens[pos].getTokenName() == "return") { //return
        stmtNode.addLeaf(tokens[pos],pos);pos++;
        if (tokens[pos].getType() == TokenType::PLUS ||
            tokens[pos].getType() == TokenType::MINU ||
            tokens[pos].getType() == TokenType::NOT ||
            tokens[pos].getType() == TokenType::IDENFR ||
            tokens[pos].getType() == TokenType::LPARENT ||
            tokens[pos].getType() == TokenType::INTCON) {
            pos = parseExp(pos,&stmtNode);
        }
        if (tokens[pos].getTokenName() == ";") {
            stmtNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    else if (tokens[pos].getTokenName() == "printf") { //print
        stmtNode.addLeaf(tokens[pos],pos);pos++;//printf
        stmtNode.addLeaf(tokens[pos],pos);pos++;//(
        stmtNode.addLeaf(tokens[pos],pos);pos++;//formatString
        while(tokens[pos].getTokenName() == ",") {
            stmtNode.addLeaf(tokens[pos],pos);pos++;
            pos = parseExp(pos,&stmtNode);
        }
        if (tokens[pos].getTokenName() == ")") {
            stmtNode.addLeaf(tokens[pos],pos);pos++;//)
        }
        if (tokens[pos].getTokenName() == ";") {
            stmtNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    else if (tokens[pos].getTokenName() == "{") { //block
        pos = parseBlock(pos,&stmtNode);
    }
    else if (tokens[pos].getTokenName() == ";") {
        stmtNode.addLeaf(tokens[pos],pos);pos++;
    }
    else if (tokens[pos].getTokenName() == "(" || tokens[pos].getType() == TokenType::INTCON
    || (tokens[pos].getType() == TokenType::IDENFR && tokens[pos + 1].getTokenName() == "(")
    || (tokens[pos].getTokenName() == "!" || tokens[pos].getTokenName() == "+" || tokens[pos].getTokenName() == "-")) {//exp
        pos = parseExp(pos,&stmtNode);
        if (tokens[pos].getTokenName() == ";") {
            stmtNode.addLeaf(tokens[pos],pos);pos++;
        }
    }
    else {
        int tmpPos = 0;
        Node noMeanNode(nullptr, GrammarType::NullType, -1);
        tmpPos = parseLVal(pos, &noMeanNode);
        if (tokens[tmpPos].getTokenName() == "=") {//LVal
            pos = parseLVal(pos, &stmtNode); //LVal
            stmtNode.addLeaf(tokens[pos],pos);pos++; // =
            if (tokens[pos].getTokenType() == TokenType::GETINTTK) { //Lval = getint();
                stmtNode.addLeaf(tokens[pos],pos);pos++;//getint
                stmtNode.addLeaf(tokens[pos],pos);pos++;//(
                if (tokens[pos].getTokenName() == ")") {
                    stmtNode.addLeaf(tokens[pos],pos);pos++;//)
                }
                if (tokens[pos].getTokenName() == ";") {//;
                    stmtNode.addLeaf(tokens[pos],pos);pos++;
                }
            }
            else { //LVal = EXP;
                pos = parseExp(pos,&stmtNode);
                if (tokens[pos].getTokenName() == ";") {//;
                    stmtNode.addLeaf(tokens[pos],pos);pos++;
                }
            }
        }
        else {
            pos = parseExp(pos,&stmtNode);
            if (tokens[pos].getTokenName() == ";") {//;
                stmtNode.addLeaf(tokens[pos],pos);pos++;
            }
        }
    }

//    else {
//        if (tokens[pos + 1].getTokenName() == "=") {
//            pos = parseLVal(pos,&stmtNode); //LVal
//            stmtNode.addLeaf(tokens[pos],pos);pos++;//=
//            if (tokens[pos].getType() == TokenType::GETINTTK) {
//                stmtNode.addLeaf(tokens[pos],pos);pos++;//getint
//                stmtNode.addLeaf(tokens[pos],pos);pos++;//(
//                if (tokens[pos].getTokenName() == ")") {
//                    stmtNode.addLeaf(tokens[pos],pos);pos++;//)
//                }
//                if (tokens[pos].getTokenName() == ";") {//;
//                    stmtNode.addLeaf(tokens[pos],pos);pos++;
//                }
//            }
//            else {
//                pos = parseExp(pos,&stmtNode);
//                if (tokens[pos].getTokenName() == ";") {
//                    stmtNode.addLeaf(tokens[pos],pos);pos++;
//                }
//            }
//        }
//        else {
//            int tmpPos = pos;
//            while(tokens[tmpPos].getTokenName()!= "=" && tokens[tmpPos].getTokenName()!= ";") {
//                tmpPos++;
//            }
//            if (tokens[tmpPos].getTokenName() == "=") {
//                /*do LVal*/
//                pos = parseLVal(pos,&stmtNode); //LVal
//                stmtNode.addLeaf(tokens[pos],pos);pos++;//=
//                if (tokens[pos].getType() == TokenType::GETINTTK) {
//                    stmtNode.addLeaf(tokens[pos],pos);pos++;//getint
//                    stmtNode.addLeaf(tokens[pos],pos);pos++;//(
//                    if (tokens[pos].getTokenName() == ")") {
//                        stmtNode.addLeaf(tokens[pos],pos);pos++;//)
//                    }
//                    if (tokens[pos].getTokenName() == ";") {//;
//                        stmtNode.addLeaf(tokens[pos],pos);pos++;
//                    }
//                }
//                else {
//                    pos = parseExp(pos,&stmtNode);
//                    if (tokens[pos].getTokenName() == ";") {//;
//                        stmtNode.addLeaf(tokens[pos],pos);pos++;
//                    }
//                }
//            }
//            else {
//                /*do Exp*/
//                pos = parseExp(pos,&stmtNode);
//                if (tokens[pos].getTokenName() == ";") {
//                    stmtNode.addLeaf(tokens[pos],pos);pos++;
//                }
//            }
//        }
//    }
    tmpNode->addChild(stmtNode);
    return pos;
}


/*Cond → LOrExp */
int Parser::parseCond(int pos, Node *tmpNode) {
    Node condNode = Node(tmpNode,GrammarType::Cond,pos);
    pos = parseLOrExp(pos,&condNode);
    tmpNode->addChild(condNode);
    return pos;
}

/* ForStmt → LVal '=' Exp*/
int Parser::parseForStmt(int pos, Node *tmpNode) {
    Node forStmtNode = Node(tmpNode,GrammarType::ForStmt,pos);
    pos = parseLVal(pos,&forStmtNode);
    forStmtNode.addLeaf(tokens[pos],pos);pos++;
    pos = parseExp(pos,&forStmtNode);
    tmpNode->addChild(forStmtNode);
    return pos;
}

/*BType → 'int'*/
int Parser::parseBType(int pos, Node *tmpNode) {
    Node bTypeNode = Node(tmpNode,GrammarType::BType,pos);
    bTypeNode.addLeaf(tokens[pos],pos);pos++;//加入int
    tmpNode->addChild(bTypeNode);//加入到父节点之中
    return pos;
}

/*CompUnit → {Decl} {FuncDef} MainFuncDef*/
Node Parser::parse() {
    Node node = Node(nullptr,GrammarType::CompUnit,0);
    lexer.run();
    this->tokens = this->lexer.getTokens();
    int pos = 0;
    /*Decl*/
    while(pos < tokens.size() && (tokens[pos].getTokenName() == "const" || (tokens[pos].getTokenName() == "int" && tokens[pos + 1].getTokenName() != "main" && tokens[pos + 2].getTokenName() != "("))) {
        pos = parseDecl(pos,&node);
    }
    while(pos < tokens.size() && (tokens[pos].getTokenName() == "void" || tokens[pos + 1].getTokenName() != "main")) {
        pos = parseFuncDef(pos, &node);
    }
    if (pos < tokens.size() && (tokens[pos].getTokenName()== "int" && tokens[pos + 1].getTokenName() == "main")) {
        pos = parseMainFuncDef(pos, &node);
    }
    this->root = node;
    return node;
}


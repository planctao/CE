//
// Created by me on 2023/9/26.
//
#include "Parser.h"
Parser::Parser(const char *path) :lexer{path}, root{Node(nullptr,GrammarType::CompUnit)}{
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
    Node expNode = Node(tmpNode,GrammarType::Exp);
    pos = parseAddExp(pos,&expNode);
    tmpNode->addChild(expNode);
    return pos;
}

/*Number → IntConst*/
int Parser::parseNumber(int pos, Node* tmpNode) {
    Node numberNode = Node(tmpNode,GrammarType::Number);
    numberNode.addLeaf(tokens[pos]);pos++;
    tmpNode->addChild(numberNode);
    return pos;
} //ok

/*LVal → Ident {'[' Exp ']'}*/
int Parser::parseLVal(int pos, Node* tmpNode) {
    Node LValNode = Node(tmpNode,GrammarType::LVal);
    LValNode.addLeaf(tokens[pos]);pos++;
    while(tokens[pos].getValue() == "[") {
        LValNode.addLeaf(tokens[pos]);pos++;
        pos = parseExp(pos,&LValNode);
        if (tokens[pos].getValue() == "]") {
            LValNode.addLeaf(tokens[pos]);pos++;
        }
    }
    tmpNode->addChild(LValNode);
    return pos;
}

/*PrimaryExp → '(' Exp ')' | LVal | Number*/
int Parser::parsePrimaryExp(int pos, Node* tmpNode) {
    Node primaryExpNode = Node(tmpNode,GrammarType::PrimaryExp);
    if (tokens[pos].getValue() == "(") {
        primaryExpNode.addLeaf(tokens[pos]);pos++;
        pos = parseExp(pos,&primaryExpNode);
        primaryExpNode.addLeaf(tokens[pos]);pos++;
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
    Node funcRParamsNode = Node(tmpNode,GrammarType::FuncRParams);
    pos = parseExp(pos,&funcRParamsNode);
    while(tokens[pos].getValue() == ",") {
        funcRParamsNode.addLeaf(tokens[pos]);pos++;
        pos = parseExp(pos,&funcRParamsNode);
    }
    tmpNode->addChild(funcRParamsNode);
    return pos;
}

/*UnaryOp → '+' | '−' | '!'*/
int Parser::parseUnaryOp(int pos, Node *tmpNode) {
    Node unaryOpNode = Node(tmpNode,GrammarType::UnaryOp);
    unaryOpNode.addLeaf(tokens[pos]);pos++;
    tmpNode->addChild(unaryOpNode);
    return pos;
}

/*UnaryExp → PrimaryExp | Ident '(' [FuncRParams] ')' | UnaryOp UnaryExp*/
int Parser::parseUnaryExp(int pos, Node* tmpNode) {
    Node unaryExpNode = Node(tmpNode,GrammarType::UnaryExp);
    if (pos + 1 < tokens.size() && tokens[pos].getType() == TokenType::IDENFR && tokens[pos + 1].getValue() == "(") {
        unaryExpNode.addLeaf(tokens[pos]);pos++;
        unaryExpNode.addLeaf(tokens[pos]);pos++;
        if (tokens[pos].getType() == TokenType::PLUS ||
            tokens[pos].getType() == TokenType::MINU ||
            tokens[pos].getType() == TokenType::NOT ||
            tokens[pos].getType() == TokenType::IDENFR ||
            tokens[pos].getType() == TokenType::LPARENT ||
            tokens[pos].getType() == TokenType::INTCON) {
            pos = parseFuncRParams(pos,&unaryExpNode);
        }
        if (tokens[pos].getValue() == ")") {
            unaryExpNode.addLeaf(tokens[pos]);pos++;
        }
    }
    else if (tokens[pos].getValue() == "+" || tokens[pos].getValue() == "-" || tokens[pos].getValue() == "!"){
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
    Node baseMulExp = Node(nullptr,GrammarType::MulExp);
    pos = parseUnaryExp(pos,&baseMulExp);
    st.push(baseMulExp);
    while(tokens[pos].getValue() == "*" || tokens[pos].getValue() == "/" || tokens[pos].getValue() == "%") {
        if (true) {
            Node mulExpNode = Node(nullptr,GrammarType::MulExp);
            mulExpNode.addChild(st.top());st.top().setParent(&mulExpNode);
            mulExpNode.addLeaf(tokens[pos]);pos++;

            pos = parseUnaryExp(pos,&mulExpNode);
            st.push(mulExpNode);
        }
        else {
            st.top().addLeaf(tokens[pos]);pos++;
            pos = parseUnaryExp(pos,&st.top());
        }
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
    Node baseAddExp = Node(nullptr,GrammarType::AddExp);
    pos = parseMulExp(pos,&baseAddExp);
    st.push(baseAddExp);
    while(tokens[pos].getValue() == "+" || tokens[pos].getValue() == "-") {
        if (true) {
            Node addExpNode = Node(nullptr,GrammarType::AddExp);
            addExpNode.addChild(st.top());
            st.top().setParent(&addExpNode);
            addExpNode.addLeaf(tokens[pos]);pos++;
            pos = parseMulExp(pos,&addExpNode);
            st.push(addExpNode);
        }
        else {
            st.top().addLeaf(tokens[pos]);pos++;
            pos = parseMulExp(pos,&st.top());
        }
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
    Node baseRelExp = Node(nullptr,GrammarType::RelExp);
    pos = parseAddExp(pos,&baseRelExp);
    st.push(baseRelExp);
    while(tokens[pos].getValue() == "<" || tokens[pos].getValue() == ">" ||
        tokens[pos].getValue() == "<=" || tokens[pos].getValue() == ">=") {
        if (true) {
            Node relExpNode = Node(nullptr,GrammarType::RelExp);
            relExpNode.addChild(st.top());
            st.top().setParent(&relExpNode);
            relExpNode.addLeaf(tokens[pos]);pos++;
            pos = parseAddExp(pos,&relExpNode);
            st.push(relExpNode);
        }
        else {
            st.top().addLeaf(tokens[pos]);pos++;
            pos = parseAddExp(pos,&st.top());
        }
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
    Node baseEqExp = Node(nullptr,GrammarType::EqExp);
    pos = parseRelExp(pos,&baseEqExp);
    st.push(baseEqExp);
    while(tokens[pos].getValue() == "==" || tokens[pos].getValue() == "!=") {
        if (true) {
            Node EqExp = Node(nullptr,GrammarType::EqExp);
            EqExp.addChild(st.top());
            st.top().setParent(&EqExp);
            EqExp.addLeaf(tokens[pos]);pos++;
            pos = parseRelExp(pos,&EqExp);
            st.push(EqExp);
        }
        else {
            st.top().addLeaf(tokens[pos]);pos++;
            pos = parseRelExp(pos,&st.top());
        }
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
    Node baseLAndExp = Node(nullptr,GrammarType::LAndExp);
    pos = parseEqExp(pos,&baseLAndExp);
    st.push(baseLAndExp);
    while(tokens[pos].getValue() == "&&") {
        if (true) {
            Node LAndExp = Node(nullptr,GrammarType::LAndExp);
            LAndExp.addChild(st.top());
            st.top().setParent(&LAndExp);
            LAndExp.addLeaf(tokens[pos]);pos++;
            pos = parseEqExp(pos,&LAndExp);
            st.push(LAndExp);
        }
        else {
            st.top().addLeaf(tokens[pos]);pos++;
            pos = parseEqExp(pos,&st.top());
        }
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
    Node baseLOrExp = Node(nullptr,GrammarType::LOrExp);
    pos = parseLAndExp(pos,&baseLOrExp);
    st.push(baseLOrExp);
    while(tokens[pos].getValue() == "||") {
        if (true) {
            Node lOrExp = Node(nullptr,GrammarType::LOrExp);
            lOrExp.addChild(st.top());
            st.top().setParent(&lOrExp);
            lOrExp.addLeaf(tokens[pos]);pos++;
            pos = parseLAndExp(pos,&lOrExp);
            st.push(lOrExp);
        }
        else {
            st.top().addLeaf(tokens[pos]);pos++;
            pos = parseLAndExp(pos,&st.top());
        }
    }
    Node ansNode = st.top();
    st.pop();
    ansNode.setParent(tmpNode);
    tmpNode->addChild(ansNode);
    return pos;
}

/*ConstExp → AddExp*/
int Parser::parseConstExp(int pos, Node* tmpNode) {
    Node constExpNode = Node(tmpNode,GrammarType::ConstExp);
    pos = parseAddExp(pos,&constExpNode);
    tmpNode->addChild(constExpNode);
    return pos;
}//ok

/*ConstInitVal → ConstExp | '{' [ ConstInitVal { ',' ConstInitVal } ] '}'*/
int Parser::parseConstInitVal(int pos, Node* tmpNode) {
    Node constInitValNode = Node(tmpNode,GrammarType::ConstInitVal);
    if (tokens[pos].getValue() == "{") {
        constInitValNode.addLeaf(tokens[pos]);
        pos++;
        while(true) {
            if (tokens[pos].getValue() == "}") {
                constInitValNode.addLeaf(tokens[pos]);
                pos++;
                break;
            }
            else if (tokens[pos].getValue() == ",") {
                constInitValNode.addLeaf(tokens[pos]);
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
    Node constDefNode = Node(tmpNode,GrammarType::ConstDef);
    constDefNode.addLeaf(tokens[pos]);pos++;// Ident
    if (tokens[pos].getValue() == "[") {
        constDefNode.addLeaf(tokens[pos]);pos++;//[
        pos = parseConstExp(pos,&constDefNode);//constExp
        if (tokens[pos].getValue() == "]") {
            constDefNode.addLeaf(tokens[pos]);pos++;
        }
    }
    if (pos < tokens.size() && tokens[pos].getValue() == "[") {
        constDefNode.addLeaf(tokens[pos]);pos++;//[
        pos = parseConstExp(pos,&constDefNode);
        if (tokens[pos].getValue() == "]") {
            constDefNode.addLeaf(tokens[pos]);pos++;
        }
    }
    constDefNode.addLeaf(tokens[pos]);pos++;//=
    pos = parseConstInitVal(pos,&constDefNode);
    tmpNode->addChild(constDefNode);
    return pos;
} //ok

/*VarDef → Ident { '[' ConstExp ']' } | Ident { '[' ConstExp ']' } '=' InitVal */
int Parser::parseVarDef(int pos, Node* tmpNode) {
    Node varDefNode = Node(tmpNode,GrammarType::VarDef);
    varDefNode.addLeaf(tokens[pos]);pos++;//Ident
    while (tokens[pos].getValue() == "[") {
        varDefNode.addLeaf(tokens[pos]);pos++;//'['
        pos = parseConstExp(pos,&varDefNode);
        if (tokens[pos].getValue() == "]") {
            varDefNode.addLeaf(tokens[pos]);pos++;//']'
        }
    }
    if (tokens[pos].getValue() == "=") {
        varDefNode.addLeaf(tokens[pos]);pos++;//=
        pos = parseInitVal(pos,&varDefNode);
    }
    tmpNode->addChild(varDefNode);
    return pos;
}

/*InitVal → Exp | '{' [ InitVal { ',' InitVal } ] '}'*/
int Parser::parseInitVal(int pos, Node* tmpNode){
    Node initValNode = Node(tmpNode,GrammarType::InitVal);
    if (tokens[pos].getValue() == "{") {
        initValNode.addLeaf(tokens[pos]);
        pos++;
        while(true) {
            if (tokens[pos].getValue() == "}") {
                initValNode.addLeaf(tokens[pos]);
                pos++;
                break;
            }
            else if (tokens[pos].getValue() == ",") {
                initValNode.addLeaf(tokens[pos]);
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
    Node declNode = Node(tmpNode,GrammarType::Decl);
    if (tokens[pos].getValue() == "const") {//const declare
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
    Node constDeclNode = Node(tmpNode,GrammarType::ConstDecl);
    constDeclNode.addLeaf(tokens[pos]);//加入const这个终结符
    pos++;
    pos = parseBType(pos,&constDeclNode);//parse BType
    while(true) {
        pos = parseConstDef(pos,&constDeclNode);
        if (tokens[pos].getValue() == ",") {
            constDeclNode.addLeaf(tokens[pos]);
            pos++;
        }
        else if (tokens[pos].getValue() == ";") {
            constDeclNode.addLeaf(tokens[pos]);pos++;
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
    Node varDeclNode = Node(tmpNode,GrammarType::VarDecl);
    pos = parseBType(pos,&varDeclNode);//BType
    pos = parseVarDef(pos,&varDeclNode);//VarDef
    while(true) {
        if (tokens[pos].getValue() == ",") {
            varDeclNode.addLeaf(tokens[pos]);
            pos++;
            pos = parseVarDef(pos,&varDeclNode);
        }
        else if (tokens[pos].getValue() == ";"){// ;
            varDeclNode.addLeaf(tokens[pos]);pos++;
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
    Node funcDefNode(tmpNode,GrammarType::FuncDef);//FuncType
    pos = parseFuncType(pos,&funcDefNode);
    funcDefNode.addLeaf(tokens[pos]);pos++;
    funcDefNode.addLeaf(tokens[pos]);pos++;
    if (tokens[pos].getType() == TokenType::PLUS ||
        tokens[pos].getType() == TokenType::MINU ||
        tokens[pos].getType() == TokenType::NOT ||
        tokens[pos].getType() == TokenType::IDENFR ||
        tokens[pos].getType() == TokenType::LPARENT ||
        tokens[pos].getType() == TokenType::INTCON) {
        pos = parseFuncFParams(pos,&funcDefNode);//FuncFParams
    }
    if (tokens[pos].getValue() == ")") {
        funcDefNode.addLeaf(tokens[pos]);pos++;//)
    }
    pos = parseBlock(pos,&funcDefNode);
    tmpNode->addChild(funcDefNode);
    return pos;
}

/*FuncType → 'void' | 'int'*/
int Parser::parseFuncType(int pos, Node* tmpNode) {
    Node funcTypeNode = Node(tmpNode,GrammarType::FuncType);
    funcTypeNode.addLeaf(tokens[pos]);
    tmpNode->addChild(funcTypeNode);
    pos++;
    return pos;
}

/*FuncFParams → FuncFParam { ',' FuncFParam }*/
int Parser::parseFuncFParams(int pos, Node *tmpNode) {
    Node funcFParamsNode = Node(tmpNode,GrammarType::FuncFParams);
    pos = parseFuncFParam(pos,&funcFParamsNode);
    while(tokens[pos].getValue() == ",") {
        funcFParamsNode.addLeaf(tokens[pos]);
        pos++;
        pos = parseFuncFParam(pos, &funcFParamsNode);
    }
    tmpNode->addChild(funcFParamsNode);
    return pos;
}

/* FuncFParam → BType Ident ['[' ']' { '[' ConstExp ']' }] */
int Parser::parseFuncFParam(int pos,Node *tmpNode) {
    Node funcFParamNode = Node(tmpNode,GrammarType::FuncFParam);
    pos = parseBType(pos,&funcFParamNode);//BType
    funcFParamNode.addLeaf(tokens[pos]);pos++; //Ident
    if (tokens[pos].getValue() == "[") {
        funcFParamNode.addLeaf(tokens[pos]);pos++;// [
        if (tokens[pos].getValue() == "]") {
            funcFParamNode.addLeaf(tokens[pos]);pos++;
        }
        while(tokens[pos].getValue() == "[") {
            funcFParamNode.addLeaf(tokens[pos]);pos++;//[
            pos = parseConstExp(pos,&funcFParamNode);
            if (tokens[pos].getValue() == "]") {
                funcFParamNode.addLeaf(tokens[pos]);pos++;
            }
        }
    }
    tmpNode->addChild(funcFParamNode);
    return pos;
}

/* MainFuncDef → 'int' 'main' '(' ')' Block*/
int Parser::parseMainFuncDef(int pos, Node* tmpNode) {
    Node mainFuncDefNode(tmpNode,GrammarType::MainFuncDef);
    for (int i = pos;i <= pos + 3;i++) {
        mainFuncDefNode.addLeaf(tokens[i]);
    }
    pos = pos + 4;
    pos = parseBlock(pos,&mainFuncDefNode);
    tmpNode->addChild(mainFuncDefNode);
    return pos;
}

/*Block → '{' { BlockItem } '}'*/
int Parser::parseBlock(int pos, Node* tmpNode) {
    Node blockNode = Node(tmpNode,GrammarType::Block);
    blockNode.addLeaf(tokens[pos]);pos++;
    while(tokens[pos].getValue() != "}") {
        pos = parseBlockItem(pos,&blockNode);
    }
    blockNode.addLeaf(tokens[pos]);pos++;
    tmpNode->addChild(blockNode);
    return pos;
}

/*BlockItem → Decl | Stmt*/
int Parser::parseBlockItem(int pos, Node *tmpNode) {
    Node blockItemNode = Node(tmpNode,GrammarType::BlockItem);
    if (tokens[pos].getValue() == "int" || tokens[pos].getValue() == "const") {
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
    Node stmtNode = Node(tmpNode,GrammarType::Stmt);
    if(tokens[pos].getValue() == "if" || tokens[pos].getValue() == "while") {//if
        stmtNode.addLeaf(tokens[pos]);pos++; //if
        stmtNode.addLeaf(tokens[pos]);pos++; //'('

        pos = parseCond(pos,&stmtNode);
        if (tokens[pos].getValue() == ")") {
            stmtNode.addLeaf(tokens[pos]);pos++;//)
        }

        pos = parseStmt(pos,&stmtNode);
        if (tokens[pos].getValue() == "else") {
            stmtNode.addLeaf(tokens[pos]);
            pos++;
            pos = parseStmt(pos,&stmtNode);
        }
    }
    else if(tokens[pos].getValue() == "for") {//for
        stmtNode.addLeaf(tokens[pos]);pos++;
        stmtNode.addLeaf(tokens[pos]);pos++;
        if (tokens[pos].getValue() != ";") {
            pos = parseForStmt(pos,&stmtNode);
        }
        stmtNode.addLeaf(tokens[pos]);pos++;//;
        if (tokens[pos].getValue() != ";") {
            pos = parseCond(pos,&stmtNode);
        }
        stmtNode.addLeaf(tokens[pos]);pos++;//;
        if (tokens[pos].getValue() != ")") {
            pos = parseForStmt(pos,&stmtNode);
        }
        stmtNode.addLeaf(tokens[pos]);pos++;//)
        pos = parseStmt(pos,&stmtNode);
    }
    else if (tokens[pos].getValue() == "break" || tokens[pos].getValue() == "continue") {//continue or break
        stmtNode.addLeaf(tokens[pos]);pos++;
        if (tokens[pos].getValue() == ";"){
            stmtNode.addLeaf(tokens[pos]);pos++;
        }
    }
    else if (tokens[pos].getValue() == "return") { //return
        stmtNode.addLeaf(tokens[pos]);pos++;
        if (tokens[pos].getType() == TokenType::PLUS ||
            tokens[pos].getType() == TokenType::MINU ||
            tokens[pos].getType() == TokenType::NOT ||
            tokens[pos].getType() == TokenType::IDENFR ||
            tokens[pos].getType() == TokenType::LPARENT ||
            tokens[pos].getType() == TokenType::INTCON) {
            pos = parseExp(pos,&stmtNode);
        }
        if (tokens[pos].getValue() == ";") {
            stmtNode.addLeaf(tokens[pos]);pos++;
        }
    }
    else if (tokens[pos].getValue() == "printf") { //print
        stmtNode.addLeaf(tokens[pos]);pos++;//printf
        stmtNode.addLeaf(tokens[pos]);pos++;//(
        stmtNode.addLeaf(tokens[pos]);pos++;//formatString
        while(tokens[pos].getValue() == ",") {
            stmtNode.addLeaf(tokens[pos]);pos++;
            pos = parseExp(pos,&stmtNode);
        }
        if (tokens[pos].getValue() == ")") {
            stmtNode.addLeaf(tokens[pos]);pos++;//)
        }
        if (tokens[pos].getValue() == ";") {
            stmtNode.addLeaf(tokens[pos]);pos++;
        }
    }
    else if (tokens[pos].getValue() == "{") { //block
        pos = parseBlock(pos,&stmtNode);
    }
    else if (tokens[pos].getValue() == ";") {
        stmtNode.addLeaf(tokens[pos]);pos++;
    }
    else if (tokens[pos].getValue() == "(" || tokens[pos].getType() == TokenType::INTCON
    || (tokens[pos].getType() == TokenType::IDENFR && tokens[pos + 1].getValue() == "(")
    || (tokens[pos].getValue() == "!" || tokens[pos].getValue() == "+" || tokens[pos].getValue() == "-")) {//exp
        pos = parseExp(pos,&stmtNode);
        if (tokens[pos].getValue() == ";") {
            stmtNode.addLeaf(tokens[pos]);pos++;
        }
    }
    else {
        if (tokens[pos + 1].getValue() == "=") {
            pos = parseLVal(pos,&stmtNode); //LVal
            stmtNode.addLeaf(tokens[pos]);pos++;//=
            if (tokens[pos].getType() == TokenType::GETINTTK) {
                stmtNode.addLeaf(tokens[pos]);pos++;//getint
                stmtNode.addLeaf(tokens[pos]);pos++;//(
                if (tokens[pos].getValue() == ")") {
                    stmtNode.addLeaf(tokens[pos]);pos++;//)
                }
                if (tokens[pos].getValue() == ";") {//;
                    stmtNode.addLeaf(tokens[pos]);pos++;
                }
            }
            else {
                pos = parseExp(pos,&stmtNode);
                if (tokens[pos].getValue() == ";") {
                    stmtNode.addLeaf(tokens[pos]);pos++;
                }
            }
        }
        else {
            int tmpPos = pos;
            while(tokens[tmpPos].getValue()!= "=" && tokens[tmpPos].getValue()!= ";") {
                tmpPos++;
            }
            if (tokens[tmpPos].getValue() == "=") {
                /*do LVal*/
                pos = parseLVal(pos,&stmtNode); //LVal
                stmtNode.addLeaf(tokens[pos]);pos++;//=
                if (tokens[pos].getType() == TokenType::GETINTTK) {
                    stmtNode.addLeaf(tokens[pos]);pos++;//getint
                    stmtNode.addLeaf(tokens[pos]);pos++;//(
                    if (tokens[pos].getValue() == ")") {
                        stmtNode.addLeaf(tokens[pos]);pos++;//)
                    }
                    if (tokens[pos].getValue() == ";") {//;
                        stmtNode.addLeaf(tokens[pos]);pos++;
                    }
                }
                else {
                    pos = parseExp(pos,&stmtNode);
                    if (tokens[pos].getValue() == ";") {//;
                        stmtNode.addLeaf(tokens[pos]);pos++;
                    }
                }
            }
            else {
                /*do Exp*/
                pos = parseExp(pos,&stmtNode);
                if (tokens[pos].getValue() == ";") {
                    stmtNode.addLeaf(tokens[pos]);pos++;
                }
            }
        }
    }
    tmpNode->addChild(stmtNode);
    return pos;
}


/*Cond → LOrExp */
int Parser::parseCond(int pos, Node *tmpNode) {
    Node condNode = Node(tmpNode,GrammarType::Cond);
    pos = parseLOrExp(pos,&condNode);
    tmpNode->addChild(condNode);
    return pos;
}

/* ForStmt → LVal '=' Exp*/
int Parser::parseForStmt(int pos, Node *tmpNode) {
    Node forStmtNode = Node(tmpNode,GrammarType::ForStmt);
    pos = parseLVal(pos,&forStmtNode);
    forStmtNode.addLeaf(tokens[pos]);pos++;
    pos = parseExp(pos,&forStmtNode);
    tmpNode->addChild(forStmtNode);
    return pos;
}

/*BType → 'int'*/
int Parser::parseBType(int pos, Node *tmpNode) {
    Node bTypeNode = Node(tmpNode,GrammarType::BType);
    bTypeNode.addLeaf(tokens[pos]);pos++;//加入int
    tmpNode->addChild(bTypeNode);//加入到父节点之中
    return pos;
}

/*CompUnit → {Decl} {FuncDef} MainFuncDef*/
Node Parser::parse() {
    Node node = Node(nullptr,GrammarType::CompUnit);
    lexer.run();
    this->tokens = this->lexer.getTokens();
    int pos = 0;
    /*Decl*/
    while(pos < tokens.size() && (tokens[pos].getValue() == "const" || (tokens[pos].getValue() == "int" && tokens[pos + 1].getValue() != "main" && tokens[pos + 2].getValue() != "("))) {
        pos = parseDecl(pos,&node);
    }
    while(pos < tokens.size() && (tokens[pos].getValue() == "void" || tokens[pos + 1].getValue() != "main")) {
        pos = parseFuncDef(pos, &node);
    }
    if (pos < tokens.size() && (tokens[pos].getValue()== "int" && tokens[pos + 1].getValue() == "main")) {
        pos = parseMainFuncDef(pos, &node);
    }
    this->root = node;
    return node;
}

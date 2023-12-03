cmake_minimum_required(VERSION 3.15)
project(compiler)

set(CMAKE_CXX_STANDARD 17)
#set(DCMAKE_SH="CMAKE_SH-NOTFOUND")

#include_directories(${PROJECT_SOURCE_DIR}/Error)
#include_directories(${PROJECT_SOURCE_DIR}/GrammarTree)
#include_directories(${PROJECT_SOURCE_DIR}/Symbol)
#include_directories(${PROJECT_SOURCE_DIR}/Util)
#
#aux_source_directory(${PROJECT_SOURCE_DIR}/Error ERROR_SRC)
#aux_source_directory(${PROJECT_SOURCE_DIR}/GrammarTree GRAMMARTREE_SRC)
#aux_source_directory(${PROJECT_SOURCE_DIR}/Symbol SYMBOL_SRC)
#aux_source_directory(${PROJECT_SOURCE_DIR}/Util UTIL_SRC)
#
#add_executable(compiler main.cpp Lexer.cpp Lexer.h Parser.cpp Parser.h ${UTIL_SRC} ${SYMBOL_SRC} ${ERROR_SRC} ${GRAMMARTREE_SRC})
add_executable(compiler main.cpp Lexer.cpp Lexer.h Util/TokenType.h GrammarTree/Token.cpp GrammarTree/Token.h Util/utils.cpp Util/utils.h
        Parser.cpp Parser.h GrammarTree/Node.cpp GrammarTree/Node.h Util/GrammarType.h Symbol/Symbol.cpp Symbol/Symbol.h Symbol/SymbolTable.cpp Symbol/SymbolTable.h
        #ERROR
        Error/Error.cpp Error/Error.h Error/ErrorToken.cpp Error/ErrorToken.h
        #LLVM
        MidCode.cpp MidCode.h LLVM/Instruction/Instruction.cpp LLVM/Instruction/Instruction.h LLVM/Instruction/BinInstruction.cpp
        LLVM/Instruction/BinInstruction.h Util/InstructionType.h LLVM/IrValue.cpp LLVM/IrValue.h LLVM/IrUser.cpp LLVM/IrUser.h
        LLVM/Use.cpp LLVM/Use.h LLVM/BasicBlock/BasicBlock.cpp LLVM/BasicBlock/BasicBlock.h LLVM/Module/Module.cpp LLVM/Module/Module.h
        LLVM/Function/Function.cpp LLVM/Function/Function.h LLVM/Function/GlobalVar.cpp LLVM/Function/GlobalVar.h LLVM/Function/Param.cpp
        LLVM/Function/Param.h LLVM/BasicBlock/BasicBlockBuilder.cpp LLVM/BasicBlock/BasicBlockBuilder.h LLVM/Instruction/InstructionBuilder.cpp
        LLVM/Instruction/InstructionBuilder.h LLVM/Instruction/AllocInstruction.cpp LLVM/Instruction/AllocInstruction.h LLVM/Instruction/Store.cpp
        LLVM/Instruction/Store.h LLVM/ValueType/ValueType.cpp LLVM/ValueType/ValueType.h Util/IrValueType.h LLVM/ValueType/ArrayType.cpp
        LLVM/ValueType/ArrayType.h LLVM/ValueType/IntegerType.cpp LLVM/ValueType/IntegerType.h LLVM/ValueType/NumberType.cpp
        LLVM/ValueType/NumberType.h LLVM/Instruction/Load.cpp LLVM/Instruction/Load.h LLVM/Instruction/Alloc.cpp
        LLVM/Instruction/Alloc.h LLVM/ValueType/PointerType.cpp LLVM/ValueType/PointerType.h LLVM/Instruction/Call.cpp
        LLVM/Instruction/Call.h Symbol/SymbolCon.cpp Symbol/SymbolCon.h Symbol/SymbolVar.cpp Symbol/SymbolVar.h
        LLVM/Function/FunctionBuilder.cpp LLVM/Function/FunctionBuilder.h Symbol/SymbolFunc.cpp Symbol/SymbolFunc.h LLVM/Instruction/Return.cpp LLVM/Instruction/Return.h LLVM/Instruction/GEP.cpp LLVM/Instruction/GEP.h MIPS/Module/MipsModule.cpp MIPS/Module/MipsModule.h MIPS/Function/Word.cpp MIPS/Function/Word.h MIPS/Function/Asciiz.cpp MIPS/Function/Asciiz.h MIPS/Function/MipsFunction.cpp MIPS/Function/MipsFunction.h MIPS/BasicBlock/MipsBasicBlock.cpp MIPS/BasicBlock/MipsBasicBlock.h MIPS/BasicBlock/MipsBlockBuilder.cpp MIPS/BasicBlock/MipsBlockBuilder.h MIPS/Instructioin/MipsInstruction.cpp MIPS/Instructioin/MipsInstruction.h MIPS/MipsSymbolTable.cpp MIPS/MipsSymbolTable.h MIPS/MipsSymbol.cpp MIPS/MipsSymbol.h MIPS/Instructioin/MipsInstructionBuilder.cpp MIPS/Instructioin/MipsInstructionBuilder.h Util/MipsInstrType.h MIPS/Instructioin/La.cpp MIPS/Instructioin/La.h MIPS/Instructioin/Li.cpp MIPS/Instructioin/Li.h MIPS/Instructioin/Syscall.cpp MIPS/Instructioin/Syscall.h MIPS/RegFile.cpp MIPS/RegFile.h MIPS/Instructioin/Lw.cpp MIPS/Instructioin/Lw.h MIPS/Instructioin/Sw.cpp MIPS/Instructioin/Sw.h MIPS/Instructioin/Move.cpp MIPS/Instructioin/Move.h MIPS/Instructioin/Add.cpp MIPS/Instructioin/Add.h MIPS/Instructioin/Addi.cpp MIPS/Instructioin/Addi.h MIPS/Instructioin/Sub.cpp MIPS/Instructioin/Sub.h MIPS/Instructioin/Mul.cpp MIPS/Instructioin/Mul.h MIPS/Instructioin/Mfhi.cpp MIPS/Instructioin/Mfhi.h MIPS/Instructioin/Div.cpp MIPS/Instructioin/Div.h MIPS/Instructioin/Jal.cpp MIPS/Instructioin/Jal.h MIPS/Instructioin/Jr.cpp MIPS/Instructioin/Jr.h LLVM/Instruction/Icmp.cpp LLVM/Instruction/Icmp.h LLVM/Instruction/Br.cpp LLVM/Instruction/Br.h LLVM/Instruction/Zext.cpp LLVM/Instruction/Zext.h LLVM/Instruction/Trunc.cpp LLVM/Instruction/Trunc.h MIPS/Instructioin/SLL.cpp MIPS/Instructioin/SLL.h MIPS/Instructioin/Cmp.cpp MIPS/Instructioin/Cmp.h MIPS/Instructioin/Beq.cpp MIPS/Instructioin/Beq.h MIPS/Instructioin/Bne.cpp MIPS/Instructioin/Bne.h MIPS/Instructioin/J.cpp MIPS/Instructioin/J.h)
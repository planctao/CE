//
// Created by me on 2023/11/18.
//

#include "Jr.h"

Jr::Jr() : MipsInstruction(MipsInstrType::JR){

}

std::string Jr::mipsOutput() {
    return "jr $ra";
}

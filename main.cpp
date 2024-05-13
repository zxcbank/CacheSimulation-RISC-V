//
// Created by Святослав on 09.05.2024.
//
#include <iostream>
#include "include/ArgParser.h"
#include "include/processor.hpp"

int main(int argc, char **argv) {

    ArgParser parseobject("asm par");
    
    parseobject.parse(argc, argv);
    
    Processor cpu("Intel i5", parseobject.GetReplacement(), parseobject.GetBin());
    
    cpu.execute(parseobject.GetAsmFname());
    cpu.getStats();
    
//    std::terminate();
    return 0;
}
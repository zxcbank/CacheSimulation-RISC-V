//
// Created by Святослав on 10.05.2024.
//
#include "processor.hpp"
//#define ITER_DEBUG

bool is_emply(const std::string& reg) {
    for (int i = 0; i < reg.size(); ++i) {
        if (reg[i] != ' ') {
            return false;
        }
    }
    return true;
}

void Processor::execute(const std::string& filename) {
    int pc = 0; // Program Counter
    int global_time_utc0 = 0;
    asm_file.open(filename);
    std::vector<AssmblerInstruction> instructions;
    
    if (asm_file.is_open()) {
        std::string line;
          while (std::getline(asm_file, line)) {
              // загрузка списка команд
              if (is_emply(line))
                  continue;
              else {
                  AssmblerInstruction instruction = AssmblerInstruction(line, labels, pc);
                  instructions.push_back(instruction);
              }
              pc++;
          }
          
        pc = 0;
        while (pc < instructions.size()) {
            auto ins = instructions[pc];
            global_time_utc0++;
            if (ins.isMarked()) {
                pc++;
                continue;
            }
            
            if (ins.isRamUsing()) {
                int index = (registers[getRegNum(ins.getOp3())] >> 5) & 31;
                int tag = (registers[getRegNum(ins.getOp3())] >> 10) & 255;
                
                lru_cache.checkLRU(index, tag, global_time_utc0);
                plru_cache.checkPLRU(index, tag, global_time_utc0);
            }
            
            ins.invoke(pc, labels, memory, registers, instructions); //TODO:: blt рушит всю прогу я не виноват
            pc++;
        }
        asm_file.close();
    } else {
        std::cerr << "File not found" << std::endl;
    }

    #ifdef ITER_DEBUG
            std::cout << "Iterations: " << global_time_utc0 << std::endl;
    #endif
    
}

void Processor::getStats() const {
    if (replacement == 0) { // bit-pLRU и LRU
        printf("LRU\thit rate: %3.4f%%\npLRU\thit rate: %3.4f%%\n", lru_cache.hitRate(), plru_cache.hitRate());
//        lru_cache.hitRate();
    } else if (replacement == 1) { // LRU
        printf("LRU\thit rate: %3.4f%%\n", lru_cache.hitRate());
    } else if (replacement == 2) { // pLRU
        printf("pLRU\thit rate: %3.4f%%\n", plru_cache.hitRate());
    }
}

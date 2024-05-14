//
// Created by Святослав on 10.05.2024.
//
#include <iomanip>
#include "processor.hpp"
//#define ITER_DEBUG
//#define BIN_DEBUG
//#define STAT_DEBUG

#define mask32 0xffffffff
#define mask2byte_1 0xff000000
#define mask2byte_2 0x00ff0000
#define mask2byte_3 0x0000ff00
#define mask2byte_4 0x000000ff

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
    bool asm_file_open = false;
    asm_file.open(filename);
    bin_file.open(bin_filename, std::ios::binary | std::ios::out);
    
    std::vector<AssmblerInstruction> instructions;
    
    if (asm_file.is_open()) {
        asm_file_open = true;
        std::string line;
          while (std::getline(asm_file, line)) {
              // загрузка списка команд
              if (is_emply(line))
                  continue;
              else {
                  AssmblerInstruction instruction = AssmblerInstruction(line, labels, pc);
                  
                  if (instruction.isMarked()) {
                      continue;
                  }
                  instructions.push_back(instruction);
              }
              pc++;
          }
          
        pc = 0;
        while (pc < instructions.size()) {
            auto ins = instructions[pc];
            global_time_utc0++;
            
            if (ins.isRamUsing()) {
                int index = (registers[getRegNum(ins.getOp3())] >> 5) & 31;
                int tag = (registers[getRegNum(ins.getOp3())] >> 10) & 255;
                
                lru_cache.checkLRU(index, tag, global_time_utc0);
                plru_cache.checkPLRU(index, tag, global_time_utc0);
            }
            try {
                ins.invoke(pc, labels, memory, registers, instructions);
                pc++;
            } catch(...) {
                std::cout << "Error in line: " << pc << std::endl;
                exit(1);
            }
            
        }
        
        asm_file.close();
    } else {
        std::cerr << "Asm file not found \n" << std::endl;
        exit(1);
    }
    pc = 0;
    if (bin_file.is_open() && asm_file_open) {
        for (int i = 0; i < instructions.size(); ++i) {
            writeBinForm(pc ,instructions[i], instructions, bin_file);
            pc++;
        }
        
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

    #ifdef STAT_DEBUG
        std:: cout << "lru access: " << lru_cache.cacheAccess << std::endl;
        std:: cout << "lru hit: " << lru_cache.cacheHit << std::endl;
    #endif
}
bool InsUsingLabels(const AssmblerInstruction& ins, std::map<std::string, int>& labels) {
    if (labels[ins.getOp3()] != 0) {
        return true;
    }
    return false;
}

void order(int res, std::ostream& out) {
    int t_res = (res & mask2byte_4) | (res & mask2byte_3) | (res & mask2byte_2) | (res & mask2byte_1);
    out.write(reinterpret_cast<char*>(&t_res), 4);
}

void Processor::writeBinForm(int pc, const AssmblerInstruction& ins, std::vector<AssmblerInstruction>& instructions, std::ofstream& bin_file_) {
    int res;
    int opcode;
    int type;
    
    if (ins.getCommand() == "addi") {
        type = 0b11;
        opcode = 0b00100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b000;
        int c19_15 = getRegNum(ins.getOp2());
        int c31_20 = convert(ins.getOp3());
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c31_20 << 20);
        
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "add") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b000;
        int c19_15 = getRegNum(ins.getOp2());
        int c31_20 = getRegNum(ins.getOp3());
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c31_20 << 20);
        
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "srli"){
        type = 0b11;
        opcode = 0b00100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b101;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = convert(ins.getOp3()); //shamt
        int c31_25 = 0b0000000;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20);
        
        #ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "mul") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b000;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);
        
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "lw") {
        type = 0b11;
        opcode = 0b00000;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b010;
        int c19_15 = getRegNum(ins.getOp3());
        int c31_20 = convert(ins.getOp2());
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c31_20 << 20);
        
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "lui") {
        type = 0b11;
        opcode = 0b01101;
        int c11_7 =  getRegNum(ins.getOp1());
        int c31_12 = convert(ins.getOp2());
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c31_12 << 12);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sw") {
        type = 0b11;
        opcode = 0b01000;
        int c14_12 = 0b010;
        int c19_15 = getRegNum(ins.getOp3());
        int c31_25 = convert(ins.getOp2());
        int c11_7 =  getRegNum(ins.getOp2()) & 0xF;
        res = (opcode << 2) | (type) | (c14_12 << 12) | (c19_15 << 15) | ((c31_25 >> 6) << 25);
        
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "blt") {
        type = 0b11;
        opcode = 0b11000;
        int c24_20 = getRegNum(ins.getOp2());
        int c19_15 = getRegNum(ins.getOp1());
        int c14_12 = 0b100;
        int offset;
        if (!labels[ins.getOp3()]) {
            offset = convert(ins.getOp3()); //offset
        } else {
            offset =   4 * (labels[ins.getOp3()] - pc + 1);
        }
        
        res = (opcode << 2) | (type) | (c24_20 << 20) | (c19_15 << 15) | (((offset >> 1) & 0xf) << 8) | (c14_12 << 12) |
            (((offset >> 11) & 1) << 7) | (((offset >> 5) & 0x3f) << 25) | (((offset >> 12) & 1) << 31);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "auipc") {
        res = (0b11) | (0b00101) | (getRegNum(ins.getOp1()) << 7) | (convert(ins.getOp2()) << 12);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "slti") {
        res = (0b11) | (0b00100 << 2)| (0b010 << 12)| (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
                order(res, bin_file_);
    } else if (ins.getCommand() == "sltiu") {
        res = (0b11) | (0b00100 << 2)| (0b011 << 12)| (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
                order(res, bin_file_);
    } else if (ins.getCommand() == "sltiu") {
        res = (0b11) | (0b00100 << 2)| (0b100 << 12)| (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "xori") {
        res = (0b11) | (0b00100 << 2) | (0b100 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "ori") {
        res = (0b11) | (0b00100 << 2) | (0b110 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "andi") {
        res = (0b11) | (0b00100 << 2) | (0b111 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "slli") {
        res = (0b11) | (0b00100 << 2) | (0b001 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "srli") {
        res = (0b11) | (0b00100 << 2) | (0b101 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "srai") {
        res = (0b11) | (0b00100 << 2) | (0b101 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (convert(ins.getOp3()) << 20)
            | (0b01000 << 27);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sub") {
        res = (0b11) | (0b01100 << 2) | (0b000 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20)
            | (0b01000 << 27);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sll") {
        res = (0b11) | (0b01100 << 2) | (0b001 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "slt") {
        res = (0b11) | (0b01100 << 2) | (0b010 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sltu") {
        res = (0b11) | (0b01100 << 2) | (0b011 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "xor") {
        res = (0b11) | (0b01100 << 2) | (0b100 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "srl") {
        res = (0b11) | (0b01100 << 2) | (0b101 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sra") {
        res = (0b11) | (0b01100 << 2) | (0b101 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20) |
            (0b01000 << 27);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "or") {
        res = (0b11) | (0b01100 << 2) | (0b110 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "and") {
        res = (0b11) | (0b01100 << 2) | (0b111 << 12) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "ecall") {
        res = (0b11) | (0b11100 << 2);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "ebreak") {
        res = (0b11) | (0b11100 << 2) | (1 << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "uret") {
        res = (0b11) | (0b11100 << 2) | (2 << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sret") {
        res = (0b11) | (0b11100 << 2) | (2 << 20) | (0b000100 << 27);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "mret") {
        res = (0b11) | (0b11100 << 2) | (2 << 20) | (0b00110 << 27);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "lb") {
        res = (0b11) | (0b00000 << 2) | (0b000 << 12) | (getRegNum(ins.getOp1()) << 7) | (convert(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "lh") {
        
        res = (0b11) | (0b00000 << 2) | (0b001 << 12) | (getRegNum(ins.getOp1()) << 7) | (convert(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "lbu") {
        res = (0b11) | (0b00000 << 2) | (0b100 << 12) | (getRegNum(ins.getOp1()) << 7) | (convert(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "lhu") {
        res = (0b11) | (0b00000 << 2) | (0b101 << 12) | (getRegNum(ins.getOp1()) << 7) | (convert(ins.getOp2()) << 15) | (getRegNum(ins.getOp3()) << 20);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sb") {
        res = (0b11) | (0b01000 << 2) | (0b000 << 12) | ((convert(ins.getOp2()) & 0xF) << 7) | (getRegNum(ins.getOp3()) << 15) | (getRegNum(ins.getOp1()) << 20)
            | ((convert(ins.getOp2()) & 0x7F0) << 25);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "sh") {
        
        res = (0b11) | (0b01000 << 2) | (0b001 << 12) | ((convert(ins.getOp2()) & 0xF) << 7) | (getRegNum(ins.getOp3()) << 15) | (getRegNum(ins.getOp1()) << 20)
            | ((convert(ins.getOp2()) & 0x7F0) << 25);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "jal") {
        res = (0b11) | (0b11011 << 2) | (getRegNum(ins.getOp1()) << 7);
        res |= (((convert(ins.getOp2()) >> 12) & 0xFF) << 12);
        res |= (((convert(ins.getOp2()) >> 11) & 1) << 20);
        res |= ((convert(ins.getOp2()) & 0x3FF) << 21);
        res |= ((convert(ins.getOp2()) >> 20) << 31);
        #ifdef BIN_DEBUG
                bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
        #endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "jalr") {
        res = (0b11) | (0b11001 << 2) | (getRegNum(ins.getOp1()) << 7) | (getRegNum(ins.getOp2()) << 20) |
            (convert(ins.getOp3()) << 20);
    } else if (ins.getCommand() == "beq") {
        type = 0b11;
        opcode = 0b11000;
        int c24_20 = getRegNum(ins.getOp2());
        int c19_15 = getRegNum(ins.getOp1());
        int c14_12 = 0b000;
        int offset;
        if (!labels[ins.getOp3()]) {
            offset = convert(ins.getOp3()); //offset
        } else {
            offset =   4 * (labels[ins.getOp3()] - pc + 1);
        }
        
        res = (opcode << 2) | (type) | (c24_20 << 20) | (c19_15 << 15) | (((offset >> 1) & 0xf) << 8) | (c14_12 << 12) |
            (((offset >> 11) & 1) << 7) | (((offset >> 5) & 0x3f) << 25) | (((offset >> 12) & 1) << 31);
#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "bne") {
        type = 0b11;
        opcode = 0b11000;
        int c24_20 = getRegNum(ins.getOp2());
        int c19_15 = getRegNum(ins.getOp1());
        int c14_12 = 0b001;
        int offset;
        if (!labels[ins.getOp3()]) {
            offset = convert(ins.getOp3()); //offset
        } else {
            offset =   4 * (labels[ins.getOp3()] - pc + 1);
        }
        
        res = (opcode << 2) | (type) | (c24_20 << 20) | (c19_15 << 15) | (((offset >> 1) & 0xf) << 8) | (c14_12 << 12) |
            (((offset >> 11) & 1) << 7) | (((offset >> 5) & 0x3f) << 25) | (((offset >> 12) & 1) << 31);
#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "bge") {
        type = 0b11;
        opcode = 0b11000;
        int c24_20 = getRegNum(ins.getOp2());
        int c19_15 = getRegNum(ins.getOp1());
        int c14_12 = 0b101;
        int offset;
        if (!labels[ins.getOp3()]) {
            offset = convert(ins.getOp3()); //offset
        } else {
            offset =   4 * (labels[ins.getOp3()] - pc + 1);
        }
        
        res = (opcode << 2) | (type) | (c24_20 << 20) | (c19_15 << 15) | (((offset >> 1) & 0xf) << 8) | (c14_12 << 12) |
            (((offset >> 11) & 1) << 7) | (((offset >> 5) & 0x3f) << 25) | (((offset >> 12) & 1) << 31);
#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "bltu") {
        type = 0b11;
        opcode = 0b11000;
        int c24_20 = getRegNum(ins.getOp2());
        int c19_15 = getRegNum(ins.getOp1());
        int c14_12 = 0b110;
        int offset;
        if (!labels[ins.getOp3()]) {
            offset = convert(ins.getOp3()); //offset
        } else {
            offset =   4 * (labels[ins.getOp3()] - pc + 1);
        }
        
        res = (opcode << 2) | (type) | (c24_20 << 20) | (c19_15 << 15) | (((offset >> 1) & 0xf) << 8) | (c14_12 << 12) |
            (((offset >> 11) & 1) << 7) | (((offset >> 5) & 0x3f) << 25) | (((offset >> 12) & 1) << 31);
#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "bgeu") {
        type = 0b11;
        opcode = 0b11000;
        int c24_20 = getRegNum(ins.getOp2());
        int c19_15 = getRegNum(ins.getOp1());
        int c14_12 = 0b111;
        int offset;
        if (!labels[ins.getOp3()]) {
            offset = convert(ins.getOp3()); //offset
        } else {
            offset =   4 * (labels[ins.getOp3()] - pc + 1);
        }
        
        res = (opcode << 2) | (type) | (c24_20 << 20) | (c19_15 << 15) | (((offset >> 1) & 0xf) << 8) | (c14_12 << 12) |
            (((offset >> 11) & 1) << 7) | (((offset >> 5) & 0x3f) << 25) | (((offset >> 12) & 1) << 31);
#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "mulh") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b001;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);

#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "mulhsu") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b010;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);

#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "mulhu") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b011;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);

#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "div") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b100;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);

#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "divu") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b101;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);

#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "rem") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b110;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);

#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else if (ins.getCommand() == "remu") {
        type = 0b11;
        opcode = 0b01100;
        int c11_7 =  getRegNum(ins.getOp1());
        int c14_12 = 0b111;
        int c19_15 = getRegNum(ins.getOp2());
        int c24_20 = getRegNum(ins.getOp3());
        int c26_25 = 0b01;
        
        res = (opcode << 2) | (type) | (c11_7 << 7) | (c14_12 << 12) | (c19_15 << 15) | (c24_20 << 20) | (c26_25 << 25);

#ifdef BIN_DEBUG
        bin_file_ << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() << " ";
#endif
        order(res, bin_file_);
    } else {
        if (!ins.isMarked())
            std::cerr << "failed to binary for : " << ins.getCommand() << " " << ins.getOp1() << " " << ins.getOp2() << " " << ins.getOp3() <<  "\n";
        return;
    }
}

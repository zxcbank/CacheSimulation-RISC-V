//
// Created by Святослав on 10.05.2024.
//
#include <vector>
#include <map>
#include "RISK32I.hpp"

#define REG_SIZE 32

int sext(int val, int size) {
    val <<= (REG_SIZE - size);
    int res = val >> (REG_SIZE - size);
    return res;
}

void addi (std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] + sext(imm, 12);
}

void add (std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] + reg[r3];
}

void srli (std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] >> imm;
}

void lui(std::vector<int>& reg, int r1, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = sext(imm << 12, 20); // мб не 12 на самом деле потом проверю
}

void blt(std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels) { // для меток
    if (labels[r3]) {
            if (reg[r1] < reg[r2])
                pc = labels[r3];
    } else {
        if (reg[r1] < reg[r2]) {
            pc += (convert(r3) / 4) - 1;
        }
    }
}

void sw (std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    memory[reg[r1] + sext(offset, 7)] = reg[r2]; // здесь мб сигфолт будет хз
}

void lw (std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    if (r1 == 0)
        return;
    reg[r1] = memory[reg[r2] + sext(offset, 12)]; // здесь тоже
}

void auipc(std::vector<int>& reg, int r1, int imm, int pc) {
    if (r1 == 0)
        return;
    reg[r1] = sext(imm << 12, 20)  + pc;
}


void slti(std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] < sext(imm, 12);
}

void sltiu(std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] < abs(sext(imm, 12));
}


void xori(std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] ^ sext(imm, 12);
}


void ori(std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] | sext(imm, 12);
}

void andi (std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] & sext(imm, 12);
}


void slli(std::vector<int>& reg, int r1, int r2, int imm) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] << imm;
}

void sub(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] - reg[r3];


}


void sll(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] << reg[r3];
}


void slt(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] < reg[r3];
}


void sltu(std::vector<int>& reg , int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] < abs(reg[r3]);
}

void xort(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] ^ reg[r3];
}


void srl(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] >> abs(reg[r3]);
}


void sra(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] >> reg[r3];   //arithmetic shift
}


void ort(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] | reg[r3];
}


void andt(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] & reg[r3];
}

void ecall() {
    std::cout << "ecall" << std::endl;
    throw std::exception();
}

void ebreak() {
    std::cout << "ebreak" << std::endl;
    throw std::exception();
}

void uret() {
    std::cout << "uret " << std::endl;
    throw std::exception();
}

void mret() {
    std::cout << "mret " << std::endl;
    throw std::exception();
}


void lb(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    if (r1 == 0)
        return;
    reg[r1] = memory[reg[r2] + sext(offset, 7)];
}

void lh(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    if (r1 == 0)
        return;
    reg[r1] = memory[reg[r2] + sext(offset, 15)];
}

void lbu(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    if (r1 == 0)
        return;
    reg[r1] = memory[reg[r2] + sext(offset, 7) ] & 0xFF;
}

void lhu(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    if (r1 == 0)
        return;
    reg[r1] = memory[reg[r2] + sext(offset, 15)] & 0xFFFF;
}


void sb(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    memory[reg[r1] + sext(offset, 7)] = reg[r2];
}

void sh(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    memory[reg[r1] + sext(offset, 15)] = reg[r2];
}

void jal(std::vector<int>& reg, int r1, int imm, int& pc) {
    reg[r1] = pc + 4;
    pc += sext(imm, 20);
}

void jalr(std::vector<int>& reg, int r1, int r2, int offset, int& pc) {
    int t = pc + 4;
    pc = reg[r2] + sext(offset, 12) & (~1);
    reg[r1] = t;
}

void beq (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels) {
    if (labels[r3]) {
        if (reg[r1] == reg[r2])
            pc = labels[r3];
    } else {
        if (reg[r1] == reg[r2]) {
            pc += (convert(r3) / 4) - 1;
        }
    }
}


void bne (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels) {
    if (labels[r3]) {
        if (reg[r1] != reg[r2])
            pc = labels[r3];
    } else {
        if (reg[r1] != reg[r2]) {
            pc += (convert(r3) / 4) - 1;
        }
    }
}


void bge (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels) {
    if (labels[r3]) {
        if (reg[r1] >= reg[r2])
            pc = labels[r3];
    } else {
        if (reg[r1] >= reg[r2]) {
            pc += (convert(r3) / 4) - 1;
        }
    }
}

void bltu (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels) {
    if (labels[r3]) {
        if (reg[r1] < abs(reg[r2]))
            pc = labels[r3];
    } else {
        if (reg[r1] < abs(reg[r2])) {
            pc += (convert(r3) / 4) - 1;
        }
    }
}

void bgeu (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels) {
    if (labels[r3]) {
        if (reg[r1] >= abs(reg[r2]))
            pc = labels[r3];
    } else {
        if (reg[r1] >= abs(reg[r2])) {
            pc += (convert(r3) / 4) - 1;
        }
    }
}
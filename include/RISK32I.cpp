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
    reg[r1] = reg[r2] + sext(imm, 12);
}

void add (std::vector<int>& reg, int r1, int r2, int r3) {
    reg[r1] = reg[r2] + reg[r3];
}

void srli (std::vector<int>& reg, int r1, int r2, int imm) {
    reg[r1] = reg[r2] >> imm;
}

void lui(std::vector<int>& reg, int r1, int imm) {
    reg[r1] = sext(imm << 12, 20); // мб не 12 на самом деле потом проверю
}

void blt(std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels) { // для меток
    if (reg[r1] < reg[r2])
        pc = labels[r3];
}

void sw (std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    memory[reg[r1] + sext(offset, 7)] = reg[r2]; // здесь мб сигфолт будет хз
}

void lw (std::vector<int>& reg, int r1, int offset, int r2, RAM& memory) {
    
    reg[r1] = memory[reg[r2] + sext(offset, 12)]; // здесь тоже
}


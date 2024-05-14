//
// Created by Святослав on 10.05.2024.
//
#include "RISK32M.hpp"
#include <iostream>
#include <vector>
#include <cstdint>

void mul (std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = reg[r2] * reg[r3];
}


void mulh (std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = (reg[r2] * reg[r3]) >> 16;
}

void mulhsu (std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    reg[r1] = ((uint32_t)reg[r2] * reg[r3]) >> 16;
}


void div (std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    if (reg[r3] == 0) {
        std::cout << "division by zero\n";
        return;
    }
    reg[r1] = reg[r2] / reg[r3];
}

void divu (std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    if (reg[r3] == 0) {
        std::cout << "division by zero\n";
        return;
    }
    reg[r1] = (uint32_t) reg[r2] / reg[r3];
}


void rem (std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    if (reg[r3] == 0) {
        std::cout << "division by zero\n";
        return;
    }
    reg[r1] = reg[r2] % reg[r3];
}


void remu(std::vector<int>& reg, int r1, int r2, int r3) {
    if (r1 == 0)
        return;
    if (reg[r3] == 0) {
        std::cout << "division by zero\n";
        return;
    }
    reg[r1] = (uint32_t) reg[r2] % reg[r3];
}
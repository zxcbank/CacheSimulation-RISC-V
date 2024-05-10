//
// Created by Святослав on 10.05.2024.
//
#include "RISK32M.hpp"
#include <iostream>
#include <vector>

void mul (std::vector<int>& reg, int r1, int r2, int r3) {
    reg[r1] = reg[r2] * reg[r3];
}
//
// Created by Святослав on 10.05.2024.
//

#pragma once
#include <iostream>
#include <vector>

void mul (std::vector<int>& reg, int r1, int r2, int r3);

void mulh (std::vector<int>& reg, int r1, int r2, int r3);

void mulhsu(std::vector<int>& reg, int r1, int r2, int r3);

void div (std::vector<int>& reg, int r1, int r2, int r3);

void divu (std::vector<int>& reg, int r1, int r2, int r3);

void rem (std::vector<int>& reg, int r1, int r2, int r3);

void remu(std::vector<int>& reg, int r1, int r2, int r3);
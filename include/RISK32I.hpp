//
// Created by Святослав on 10.05.2024.
//

#pragma once
#include <iostream>
#include <vector>
#include "ram.hpp"
#include "RiskVcommand.hpp"

void addi (std::vector<int>& reg, int r1, int r2, int imm);

void add (std::vector<int>& reg, int r1, int r2, int r3);

void srli (std::vector<int>& reg, int r1, int r2, int imm);

void lui (std::vector<int>& reg, int r1, int imm);

void blt (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels);

void sw (std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);

void lw (std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);
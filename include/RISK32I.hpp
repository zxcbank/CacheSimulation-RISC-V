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

void auipc(std::vector<int>& reg, int r1, int imm, int pc);

void slti(std::vector<int>& reg, int r1, int r2, int imm);

void sltiu(std::vector<int>& reg, int r1, int r2, int imm);

void xori(std::vector<int>& reg, int r1, int r2, int imm);

void ori(std::vector<int>& reg, int r1, int r2, int imm);

void andi (std::vector<int>& reg, int r1, int r2, int imm);

void slli(std::vector<int>& reg, int r1, int r2, int imm);

void sub(std::vector<int>& reg, int r1, int r2, int r3);

void sll(std::vector<int>& reg, int r1, int r2, int r3);

void slt(std::vector<int>& reg, int r1, int r2, int r3);

void sltu(std::vector<int>& reg , int r1, int r2, int r3);

void xort(std::vector<int>& reg, int r1, int r2, int r3);

void srl(std::vector<int>& reg, int r1, int r2, int r3);

void sra(std::vector<int>& reg, int r1, int r2, int r3);

void ort(std::vector<int>& reg, int r1, int r2, int r3);

void andt(std::vector<int>& reg, int r1, int r2, int r3);

void ecall();

void ebreak();

void uret();

void mret();

void lb(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);

void lh(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);

void lbu(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);

void lhu(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);

void sb(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);

void sh(std::vector<int>& reg, int r1, int offset, int r2, RAM& memory);

void jal(std::vector<int>& reg, int r1, int imm, int& pc);

void jalr(std::vector<int>& reg, int r1, int r2, int offset,  int& pc);

void beq (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels);

void bne (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels);

void bge (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels);

void bltu (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels);

void bgeu (std::vector<int>& reg, int r1, int r2, std::string& r3, int& pc, std::map<std::string, int>& labels);
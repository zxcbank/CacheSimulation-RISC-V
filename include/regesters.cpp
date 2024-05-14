//
// Created by Святослав on 10.05.2024.
//
#include "regesters.hpp"
#include <string>
#include <iostream>

int getRegNum(const std::string& reg) {
    if (reg == "zero") return zero;
    if (reg == "ra") return ra;
    if (reg == "sp") return sp;
    if (reg == "gp") return gp;
    if (reg == "tp") return tp;
    if (reg == "t0") return t0;
    if (reg == "t1") return t1;
    if (reg == "t2") return t2;
    if (reg == "s0") return s0;
    if (reg == "fp") return s0; // важный момент
    if (reg == "s1") return s1;
    if (reg == "a0") return a0;
    if (reg == "a1") return a1;
    if (reg == "a2") return a2;
    if (reg == "a3") return a3;
    if (reg == "a4") return a4;
    if (reg == "a5") return a5;
    if (reg == "a6") return a6;
    if (reg == "a7") return a7;
    if (reg == "s2") return s2;
    if (reg == "s3") return s3;
    if (reg == "s4") return s4;
    if (reg == "s5") return s5;
    if (reg == "s6") return s6;
    if (reg == "s7") return s7;
    if (reg == "s8") return s8;
    if (reg == "s9") return s9;
    if (reg == "s10") return s10;
    if (reg == "s11") return s11;
    if (reg == "t3") return t3;
    if (reg == "t4") return t4;
    if (reg == "t5") return t5;
    if (reg == "t6") return t6;
    
//    std::cerr << "Invalid register: " << reg << std::endl;
    return -1;
}
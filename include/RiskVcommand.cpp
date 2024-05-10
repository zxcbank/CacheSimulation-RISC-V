//
// Created by Святослав on 10.05.2024.
//

#include "RiskVcommand.hpp"
#include <string>
#include <iostream>

bool AssmblerInstruction::isLabel(const std::string& line, std::map<std::string, int>& labels, int pc) {
    if (line.find(':') != std::string::npos) {
        return true;
    }
    return false;
}

AssmblerInstruction::AssmblerInstruction(const std::string& line, std::map<std::string, int>& labels, int pc) {
    
    if (isLabel(line, labels, pc)) {
        int i = 0;
        while (line[i] == ' ') i++;
        int j = i;
        while (line[j] != ':' && j < line.size()) j++;
        labels[line.substr(i, j - i)] = pc;
        isMark = true;
        return;
    }
    int i = 0;
    while (line[i] == ' ') i++;
    int j  = i;
    while (line[j] != ' ' && j < line.size()) j++;
    command = line.substr(i, j - i);
    
    while (line[j] == ' ') j++;
    i = j;
    while ((line[j] != ',' && line[j] != ' ') && j < line.size()) j++;
    op1 = line.substr(i, j - i);
    while (line[j] == ' ' || line[j] == ',') j++;
    i = j;
    while ((line[j] != ',' && line[j] != ' ') && j < line.size()) j++;
    op2 = line.substr(i, j - i );
    while (line[j] == ' ' || line[j] == ',') j++;
    i = j;
    while ((line[j] != ',' && line[j] != ' ') && j < line.size()) j++;
    op3 = line.substr(i, j - i);
    
}
int convert(const std::string& str) {
    if (str.find("0x") != std::string::npos) {
        return std::stoi(str, 0, 16);
    } else if (str.find("0b") != std::string::npos) {
        return std::stoi(str, 0, 2);
    } else {
        return std::stoi(str);
    }
}

void AssmblerInstruction::invoke(int& pc, std::map<std::string, int>& labels, RAM& memory, std::vector<int>& reg, std::vector<AssmblerInstruction>& instructions) {
    if (command == "addi") {
        addi(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "add") {
        add(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "srli") {
        srli(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "lui") {
        lui (reg, getRegNum(op1), convert(op2));
    } else if (command == "blt") {
        blt (reg, getRegNum(op1), getRegNum(op2), op3, pc, labels);
    } else if (command == "mul") {
        mul(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "sw") {
        sw(reg, getRegNum(op1), convert(op2), getRegNum(op3), memory);
    } else if(command == "lw") {
        lw(reg, getRegNum(op1), convert(op2),getRegNum(op3), memory);
    } else {
        std::cerr << "Unknown command" << std::endl;
    }
}

bool AssmblerInstruction::isRamUsing() {
    if (std::ranges::find(ramCommands.begin(), ramCommands.end(), command) != ramCommands.end()) {
        return true;
    }
    return false;
}
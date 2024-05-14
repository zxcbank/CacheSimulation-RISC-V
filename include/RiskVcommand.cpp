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
        labels[line.substr(i, j - i)] = pc - 1;
        isMark = true;
        label = line.substr(i, j - i);
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
    } else if (command == "auipc") {
        auipc(reg, getRegNum(op1), convert(op2), pc);
    } else if (command == "slti") {
        slti(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "sltiu") {
        sltiu(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "xori") {
        xori(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "ori") {
        ori(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "andi") {
        andi(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "slli") {
        slli(reg, getRegNum(op1), getRegNum(op2), convert(op3));
    } else if (command == "sub") {
        sub(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "sll") {
        sll(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "slt") {
        slt(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "sltu") {
        sltu(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "xor") {
        xort(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "srl") {
        srl(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "sra") {
        sra(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "or") {
        ort(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "and") {
        andt(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "ecall") {
        ecall();
    } else if (command == "ebreak") {
        ebreak();
    } else if (command == "uret") {
        uret();
    } else if (command == "mret") {
        mret();
    } else if (command == "lb") {
        lb(reg, getRegNum(op1), convert(op2), getRegNum(op3), memory);
    } else if (command == "lh") {
        lh(reg, getRegNum(op1), convert(op2), getRegNum(op3), memory);
    } else if (command == "lbu") {
        lbu(reg, getRegNum(op1), convert(op2), getRegNum(op3), memory);
    } else if (command == "lhu") {
        lhu(reg, getRegNum(op1), convert(op2), getRegNum(op3), memory);
    } else if (command == "sb") {
        sb(reg, getRegNum(op1), convert(op2), getRegNum(op3), memory);
    } else if (command == "sh") {
        sh(reg, getRegNum(op1), convert(op2), getRegNum(op3), memory);
    } else if (command == "jal") {
        jal(reg, getRegNum(op1), convert(op2), pc);
    } else if (command == "jalr") {
        jalr(reg, getRegNum(op1), getRegNum(op2), convert(op3), pc);
    } else if (command == "beq") {
        beq(reg, getRegNum(op1), getRegNum(op2), op3, pc, labels);
    } else if (command == "bne") {
        bne(reg, getRegNum(op1), getRegNum(op2), op3, pc, labels);
    } else if (command == "bge") {
        bge(reg, getRegNum(op1), getRegNum(op2), op3, pc, labels);
    } else if (command == "bgeu") {
        bgeu(reg, getRegNum(op1), getRegNum(op2), op3, pc, labels);
    } else if (command == "mulh") {
        mulh(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "mulhsu") {
        mulhsu(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "div") {
        div(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "divu") {
        divu(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
    } else if (command == "rem") {
        rem(reg, getRegNum(op1), getRegNum(op2), getRegNum(op3));
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
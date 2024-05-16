//
// Created by Святослав on 10.05.2024.
//
#pragma once
#include <iostream>
#include <vector>
#include <map>
#include "ram.hpp"
#include "regesters.hpp"
#include "RISK32I.hpp"
#include "RISK32M.hpp"

static const std::vector<std::string> ramCommands{"lb", "lh", "lw", "sb", "sh", "sw"};

class AssmblerInstruction {
    private:
        std::string label;
        std::string command;
        std::string op1, op2, op3;
        bool isMark = false;
    public:
        
        explicit AssmblerInstruction(const std::string& line, std::map<std::string, int>& labels, int pc); //TODO: implement
        ~AssmblerInstruction() = default;
        void invoke(int& pc, std::map<std::string, int>& labels, RAM& memory, std::vector<int>& reg, std::vector<AssmblerInstruction>& instructions);
        static bool isLabel(const std::string& line, std::map<std::string, int>& labels, int i);
        bool isRamUsing();
       
    [[nodiscard]] std::string getCommand() const {
        return command;
    }
    [[nodiscard]] std::string getOp1() const {
        return op1;
    }
    [[nodiscard]] std::string getOp2() const {
        return op2;
    }
    [[nodiscard]] std::string getOp3() const {
        return op3;
    }
    [[nodiscard]] bool isMarked() const {
        return isMark;
    }
    [[nodiscard]] std::string getLabel() const {
        return label;
    }
};

int convert(const std::string& str);
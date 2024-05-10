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

static const std::vector<std::string> ramCommands{"ls", "lw", "sw"};

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
       
    std::string getCommand() {
        return command;
    }
    std::string getOp1() {
        return op1;
    }
    std::string getOp2() {
        return op2;
    }
    std::string getOp3() {
        return op3;
    }
    [[nodiscard]] bool isMarked() const {
        return isMark;
    }
};

int convert(const std::string& str);
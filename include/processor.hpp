//
// Created by Святослав on 10.05.2024.
//
#pragma once
#include <vector>
#include <iostream>
#include <map>
#include "regesters.hpp"
#include "fstream"
#include "RiskVcommand.hpp"
#include "Lru.hpp"
#include "Plru.hpp"

class Processor {
        std::ifstream asm_file;
        std::ofstream bin_file;
        std::vector<int> registers = std::vector<int>(32, 0);
        RAM memory = RAM("dd4");
        std::map<std::string, int> labels;
        LRU lru_cache;
        
        PLRU plru_cache;
        int replacement;
        std::string bin_filename;
        
    public:
        std::string name;
        [[nodiscard]] explicit Processor(std::string name, int repl, std::string bin_filename) :
            name(std::move(name)),
            replacement(repl),
            bin_filename(std::move(bin_filename)) {}
        void execute(const std::string& filename);
        void writeBinForm(int pc, const AssmblerInstruction& ins, std::vector<AssmblerInstruction>& instructions, std::ofstream& bin_file_);
        void getStats() const ;
        ~Processor() = default;
};

bool is_emply(const std::string& reg);
bool InsUsingLabels(AssmblerInstruction& ins, std::map<std::string, int>& labels);